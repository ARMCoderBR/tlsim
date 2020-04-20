/*
 * board.c
 *
 *  Created on: 28 de mar de 2020
 *      Author: milton
 */

#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "board.h"
#include "bitswitch.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define LINHAS_JANELA2 8
#define LINHAS_JANELA2B (LINHAS_JANELA2+2)

int TERM_LINES = 0;
int TERM_COLS = 0;
fd_set readfds;

WINDOW *janela0;
WINDOW *janela1;
WINDOW *janela2;
WINDOW *janela3;

pthread_mutex_t transitionmutex = PTHREAD_MUTEX_INITIALIZER;

////////////////////////////////////////////////////////////////////////////////
void desenha_janelas(void)
{
    /*box(janela0, 0 , 0);  */      /* 0, 0 gives default characters
                                 * for the vertical and horizontal
                                 * lines            */
    wrefresh(janela0);

    wclear(janela1);
    wrefresh(janela1);

    box(janela2, 0 , 0);        /* 0, 0 gives default characters
                                 * for the vertical and horizontal
                                 * lines            */
    wrefresh(janela2);
}

////////////////////////////////////////////////////////////////////////////////
void restart_handlers(void)
{
    struct timeval tv;

    FD_ZERO (&readfds);
    FD_SET(0,&readfds);
    //FD_SET(tc_ipccom_ctx.piperx,&readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 100000;    // 100 ms

    select (1/*+tc_ipccom_ctx.piperx*/,&readfds,NULL,NULL,&tv);
}

////////////////////////////////////////////////////////////////////////////////
int received_key(void)
{
    return (FD_ISSET(0,&readfds));
}

////////////////////////////////////////////////////////////////////////////////
int read_key(void)
{
    return wgetch(janela1);
}

////////////////////////////////////////////////////////////////////////////////
void sigterm_handler(int sig){

    endwin();
    exit(0);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ehandler *clock_event_handler_root = NULL;
int clock_last_val = 0;

void clock_set_val(int val){

    logger("==>clock_set_val:%d",val);

    if (val)
        clock_last_val = 1;
    else
        clock_last_val = 0;

    event e;
    e.event_handler_root = clock_event_handler_root;
    e.valueptr = &clock_last_val;
    e.timestamp = 0;
    event_insert(&e);

//    ehandler *e = clock_event_handler_root;
//
//    while (e){
//
//        e->objdest_event_handler(e->objdest, &val, 0);
//        e = e->next;
//    }
}


void board_clock_connect(void *objdest, void (*objdest_event_handler)(void *objdest, int *valptr, int timestamp)){

    ehandler *newe = malloc(sizeof(ehandler));
    if (!newe)
        return;

    newe->objdest_event_handler = objdest_event_handler;
    newe->objdest = objdest;
    newe->next = NULL;

    ehandler *e = clock_event_handler_root;
    if (e){

        while (e->next)
            e = e->next;

        e->next = newe;
    }
    else
        clock_event_handler_root = newe;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define NOBJ 1000

pthread_mutex_t ncursesmutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t refthread;
int refresh_run = 0;
int must_refresh = 0;

////////////////////////////////////////////////////////////////////////////////
void board_set_refresh(){

    must_refresh = 1;
}

////////////////////////////////////////////////////////////////////////////////
void rectangle(int y1, int x1, int y2, int x2)
{
    mvwhline(janela1, y1, x1, 0, x2-x1);
    mvwhline(janela1, y2, x1, 0, x2-x1);
    mvwvline(janela1, y1, x1, 0, y2-y1);
    mvwvline(janela1, y1, x2, 0, y2-y1);
    mvwaddch(janela1, y1, x1, ACS_ULCORNER);
    mvwaddch(janela1, y2, x1, ACS_LLCORNER);
    mvwaddch(janela1, y1, x2, ACS_URCORNER);
    mvwaddch(janela1, y2, x2, ACS_LRCORNER);
}

////////////////////////////////////////////////////////////////////////////////
void board_refresh_a(board_object *b, int new_h, int new_w){

    if (b->type != BOARD) return;   // Erro interno - nunca deve acontecer.

    rectangle(new_h, new_w, new_h+b->w_height-1, new_w+b->w_width-1);

    if (b->name[0]){

        wmove(janela1, new_h, 1 + new_w);
        waddch(janela1,'[');
        waddstr(janela1,b->name);
        waddch(janela1,']');
    }

    b = b->objptr_root;

    while (b){

        wmove(janela1, new_h + b->pos_h, new_w + b->pos_w);

        switch (b->type){

        case MANUAL_SWITCH:
            {
                bitswitch* bs = b->objptr;

                if (bs->value)
                    waddstr(janela1,"[0 >1]");
                else
                    waddstr(janela1,"[0< 1]");
            }
            break;

        case LED:
            {
                indicator* out = b->objptr;
                if (out->value)
                    waddstr(janela1,"[#]");
                else
                    waddstr(janela1,"[.]");
            }
            break;

        case XDIGIT:
            {
                indicator* out = b->objptr;
                char s[10];
                sprintf(s,"%X",out->value & 0x0F);
                waddstr(janela1,s);
            }
            break;

        case BOARD:

            board_refresh_a(b/*->objptr_root*/,b->pos_h, b->pos_w);
            break;
        }

        if (b->type != BOARD){

            wmove(janela1,1 + new_h + b->pos_h, new_w + b->pos_w);
            waddstr(janela1,b->name);
        }

        if (b->type == MANUAL_SWITCH){

            int key = b->key;
            char s[10];
            if ((key >= KEY_F(1)) && (key <= KEY_F(12))){

                sprintf(s,"[F%d]",1+key-KEY_F(1));
            }
            else
                sprintf(s,"[%c]",key);

            waddstr(janela1,s);
        }

        b = b->objptr_next;
    }
}

////////////////////////////////////////////////////////////////////////////////
void board_refresh(board_object *b){

    pthread_mutex_lock(&ncursesmutex);

    board_refresh_a(b,0,0);

    wrefresh(janela1);
    wrefresh(janela0);

    pthread_mutex_unlock(&ncursesmutex);
}

////////////////////////////////////////////////////////////////////////////////
void *refresh_thread(void *args){

    board_object * refboard = args;

    refresh_run = 1;

    while (refresh_run){

        if (must_refresh)
            board_refresh(refboard);

        must_refresh = 0;
        usleep(42000);      //42 ms
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void refresh_thread_stop(){

    refresh_run = 0;
    pthread_join(refthread,NULL);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


pthread_t clkthread;

int clock_run = 0;
int clock_pausing = 0;
int clock_state_paused = 0;
int clock_period_us = 500000;
int iclk = 0;

#define NCLKS 10

int CLKS_PERIOD_US[NCLKS] = {
                             2000000,    //2s
                             1000000,    //1s
                             500000,     //500ms
                             250000,     //250ms
                             100000,     //100ms
                             50000,      //50ms
                             10000,      //10ms
                             1000,       //1ms
                             100,        //100us
                             10,         //10us
};

////////////////////////////////////////////////////////////////////////////////
void clock_redraw(){

    pthread_mutex_lock(&ncursesmutex);

    int i;

    char s[50] = "Clk:[";

    for (i = 0; i < NCLKS; i++)
        if (i != iclk)
            strcat(s,"-");
        else
            strcat(s,"|");
    strcat(s,"]");

    wmove(janela3,1,1);
    waddstr(janela3,s);

    wmove(janela3,2,1);
    if (!clock_pausing)
        waddstr(janela3,"RUNNING");
    else
        waddstr(janela3,"PAUSED ");

    wrefresh(janela3);

    pthread_mutex_unlock(&ncursesmutex);
}

int clock_pulse = 0;

////////////////////////////////////////////////////////////////////////////////
void *clock_thread(void *args){

    clock_period_us = CLKS_PERIOD_US[iclk];
    clock_run = 1;

    clock_redraw();

    while (clock_run){

        if (clock_event_handler_root){

            if (clock_pausing){

                clock_state_paused = 1;

                if (clock_pulse){

                    clock_pulse = 0;

                    pthread_mutex_lock(&transitionmutex);

                    if (clock_last_val)
                        clock_set_val(0);
                    else
                        clock_set_val(1);

                    pthread_mutex_unlock(&transitionmutex);

                    board_set_refresh();
                    usleep(10000);
                }
                else
                    usleep(100000);
                continue;
            }
            else
                clock_state_paused = 0;

            pthread_mutex_lock(&transitionmutex);

            if (clock_last_val)
                clock_set_val(0);
            else
                clock_set_val(1);

            pthread_mutex_unlock(&transitionmutex);

            board_set_refresh();
            usleep(clock_period_us/2);
        }
        else
            usleep(100000);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void clock_faster(){

    if (clock_pausing){

        clock_pausing = 0;
        clock_redraw();
        return;
    }

    if (iclk < (NCLKS-1)){

        iclk++;
        clock_period_us = CLKS_PERIOD_US[iclk];
        clock_redraw();
    }
}

////////////////////////////////////////////////////////////////////////////////
void clock_slower(){

    if (clock_pausing){

        clock_pausing = 0;
        clock_redraw();
        return;
    }

    if (iclk > 0){

        iclk--;
        clock_period_us = CLKS_PERIOD_US[iclk];
        clock_redraw();
    }
}

////////////////////////////////////////////////////////////////////////////////
void clock_pause(){

    if (!clock_pausing){

        clock_pausing = 1;

        while (!clock_state_paused)
            usleep(100000);

        clock_redraw();
        return;
    }
    else{

        clock_pulse = 1;
//        pthread_mutex_lock(&transitionmutex);
//
//        if (clock_last_val)
//            clock_set_val(0);
//        else
//            clock_set_val(1);
//
//        pthread_mutex_unlock(&transitionmutex);
//        board_set_refresh();
        usleep(10000);
    }

    clock_redraw();
}

////////////////////////////////////////////////////////////////////////////////
void clock_thread_stop(){

    clock_run = 0;
    pthread_join(clkthread,NULL);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

board_object *board_create(int width, int height, int key, char *name){

    board_object *b = malloc(sizeof(board_object));

    if (!b) return b;

    b->pos_w = b->pos_h = 0;
    b->type = BOARD;
    b->objptr = NULL;
    b->key = key;
    if (name)
        strncpy(b->name, name, NAMESIZE);
    else
        b->name[0] = 0;
    b->w_width = width;
    b->w_height = height;
    b->objptr_root = NULL;
    b->objptr_next = NULL;
    b->board_on_focus = b;  // Focada nela própria no início.

    return b;
}

////////////////////////////////////////////////////////////////////////////////
board_object *mainboard_create(char *name){

    return board_create(0, 0, 0, name);
}

////////////////////////////////////////////////////////////////////////////////
int board_add_object(board_object *b, board_object *newobject){

    if (!b) return -2;
    if (!newobject) return -2;

    if (b->type != BOARD) return -10;

    board_object *pb = b->objptr_root;

    if (!pb)
        b->objptr_root = newobject;
    else
    while(b->objptr_next)
        b = b->objptr_next;

    b->objptr_next = newobject;

//    if (newobject->type == BOARD)
//    if (b->board_on_focus == b)
//        b->board_on_focus = newobject;

    newobject->objptr_next = NULL;  // Dupla garantia.

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int board_add_manual_switch(board_object *b, bitswitch *bs, int pos_w, int pos_h, int key, char *name){

/*
  F1[0 >1]
  UP/DOWN

  F1[0< 1]
  UP/DOWN
*/
    if (!b) return -2;
    if (!bs) return -2;

    board_object *obja = malloc(sizeof(board_object));
    if (!obja) return -1;

    obja->pos_w  = pos_w;
    obja->pos_h  = pos_h;
    obja->type   = MANUAL_SWITCH;
    obja->objptr = bs;
    obja->key    = key;
    if (name)
        strncpy(obja->name, name, NAMESIZE);
    else
        obja->name[0] = 0;
    obja->objptr_root = NULL;
    obja->objptr_next = NULL;

    return board_add_object(b, obja);
}

////////////////////////////////////////////////////////////////////////////////
int board_add_led(board_object *b, indicator *out, int pos_w, int pos_h, char *name){

/*
  [.]
  STATUS

  [#]
  STATUS
*/
    if (!b) return -2;
    if (!out) return -2;

    board_object *obja = malloc(sizeof(board_object));
    if (!obja) return -1;

    obja->pos_w  = pos_w;
    obja->pos_h  = pos_h;
    obja->type   = LED;
    obja->objptr = out;
    obja->key    = 0;
    if (name)
        strncpy(obja->name, name, NAMESIZE);
    else
        obja->name[0] = 0;
    obja->objptr_root = NULL;
    obja->objptr_next = NULL;

    return board_add_object(b, obja);
}

////////////////////////////////////////////////////////////////////////////////
int board_add_xdigit(board_object *b, indicator *out, int pos_w, int pos_h, char *name){

/*
  A
  STATUS
*/
    if (!b) return -2;
    if (!out) return -2;

    board_object *obja = malloc(sizeof(board_object));
    if (!obja) return -1;

    obja->pos_w  = pos_w;
    obja->pos_h  = pos_h;
    obja->type   = XDIGIT;
    obja->objptr = out;
    obja->key    = 0;
    if (name)
        strncpy(obja->name, name, NAMESIZE);
    else
        obja->name[0] = 0;
    obja->objptr_root = NULL;
    obja->objptr_next = NULL;

    return board_add_object(b, obja);
}

////////////////////////////////////////////////////////////////////////////////
int board_add_board(board_object *b, board_object *board, int pos_w, int pos_h){

    if (!b) return -2;
    if (!board) return -2;

//    board_object *obja = malloc(sizeof(board_object));
//    if (!obja) return -1;
//
//    obja->pos_w  = pos_w;
//    obja->pos_h  = pos_h;
//    obja->type   = BOARD;
//    obja->objptr = board;
//    obja->key    = board->key;
//    strncpy(obja->name, board->name, NAMESIZE);
//    obja->objptr_root = NULL;
//    obja->objptr_next = NULL;
//
//    return board_add_object(b, obja);

    board->pos_w = pos_w;
    board->pos_h = pos_h;
    return board_add_object(b, board);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int board_run(board_object *board){

    int resize = 0;

    if (!board) return -2;

    if (board->type != BOARD)
        return -10;

    signal (SIGHUP,SIG_IGN);
    signal (SIGINT,SIG_IGN);
    signal (SIGTERM,sigterm_handler);
    signal (SIGTSTP,SIG_IGN);
    //signal (SIGWINCH,sigwinch_handler);
    signal (SIGWINCH,SIG_IGN);

    if (initscr() == NULL){

        printf("initscr()\n");
        exit(-1);
    }

    cbreak(); noecho();            /* Inicia o NCURSES */
    ESCDELAY=200;
    TERM_LINES = LINES;
    TERM_COLS = COLS;

    janela0 = newwin(TERM_LINES,TERM_COLS,0,0);
    //janela1 = newwin(TERM_LINES-1-LINHAS_JANELA2B,TERM_COLS-2,1,1);
    janela1 = newwin(TERM_LINES-LINHAS_JANELA2B,TERM_COLS,0,0);
    janela2 = newwin(LINHAS_JANELA2B,TERM_COLS,TERM_LINES-LINHAS_JANELA2B,0);
    janela3 = newwin(LINHAS_JANELA2,TERM_COLS-2,1+TERM_LINES-LINHAS_JANELA2B,1);


    if (!board->w_width)
        board->w_width = TERM_COLS;

    if (!board->w_height)
        board->w_height = TERM_LINES-LINHAS_JANELA2B;

    //printf("4\n");
    desenha_janelas();

    //printf("5\n");

    keypad(janela1,TRUE);

    int stoprun = 0;

    pthread_create(&refthread, NULL, refresh_thread, board);
    pthread_create(&clkthread, NULL, clock_thread, NULL);

    board_set_refresh();

    while (!stoprun){

        if (resize){

            pthread_mutex_lock(&ncursesmutex);

            getmaxyx(stdscr, TERM_LINES, TERM_COLS);

            wresize(janela0,TERM_LINES,TERM_COLS);
            wresize(janela1,TERM_LINES-LINHAS_JANELA2B,TERM_COLS);
            wresize(janela2,LINHAS_JANELA2B,TERM_COLS);
            mvwin(janela2,TERM_LINES-LINHAS_JANELA2B,0);
            wresize(janela3,LINHAS_JANELA2,TERM_COLS-2);
            mvwin(janela3,1+TERM_LINES-LINHAS_JANELA2B,1);

            board->w_width = TERM_COLS;
            board->w_height = TERM_LINES-LINHAS_JANELA2B;
            desenha_janelas();
            resize = 0;

            pthread_mutex_unlock(&ncursesmutex);
            board_set_refresh();
        }

        restart_handlers();

        if (received_key()){

            int key = read_key();

            switch(key){

            case KEY_RESIZE:
                resize = 1;
                break;
            case 27:
                stoprun = 1;
                break;
            case '+':
            case '=':
                clock_faster();
                break;
            case '-':
                clock_slower();
                break;
            case 'p':
                clock_pause();
                break;
            }

            board_object *pboardfocused = board->board_on_focus;
            board_object *p;

            if (pboardfocused)
                p = pboardfocused->objptr_root;
            else
                p = NULL;

            while(p){

                if (p->type == MANUAL_SWITCH){

                    if (p->key == key){

                        pthread_mutex_lock(&transitionmutex);
                        bitswitch *bs = p->objptr;
                        bitswitch_setval(bs, 1 ^ bs->value);
                        pthread_mutex_unlock(&transitionmutex);

                        board_set_refresh();
                    }
                }
                p = p->objptr_next;
            }
        }
    }

    refresh_thread_stop();
    clock_thread_stop();
    pthread_mutex_destroy(&transitionmutex);

    endwin();
    return 0;
}
