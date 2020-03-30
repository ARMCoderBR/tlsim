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

int TERM_LINES = 0;
int TERM_COLS = 0;
fd_set readfds;

WINDOW *janela0;
WINDOW *janela1;
WINDOW *janela2;
WINDOW *janela3;

////////////////////////////////////////////////////////////////////////////////
void desenha_janelas(void)
{
    box(janela0, 0 , 0);        /* 0, 0 gives default characters
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

int board_w = 0;
int board_h = 0;
int nobjs = 0;

typedef enum {

    MANUAL_SWITCH,
    LED,
    XDIGIT,
} control_type;

typedef struct {

    int pos_w;
    int pos_h;
    control_type type;
    void *objptr;
    int key;
    char *name;
} board_object;

bitswitch *switch_to_clock = NULL;

#define NOBJ 100

board_object obja[NOBJ];

pthread_mutex_t ncursesmutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t refthread;
int refresh_run = 0;
int must_refresh = 0;

////////////////////////////////////////////////////////////////////////////////
void board_set_refresh(){

    must_refresh = 1;
}

////////////////////////////////////////////////////////////////////////////////
void board_refresh(){

    pthread_mutex_lock(&ncursesmutex);

    int i;

    for (i = 0; i < nobjs; i++){

        wmove(janela1,obja[i].pos_h,obja[i].pos_w);

        switch (obja[i].type){

        case MANUAL_SWITCH:
            {
                bitswitch* bs = obja[i].objptr;

                int key = obja[i].key;
                char s[10];
                if ((key >= KEY_F(1)) && (key <= KEY_F(12))){

                    sprintf(s,"[F%d]",1+key-KEY_F(1));
                }
                else
                    sprintf(s,"[%c]",key);

                waddstr(janela1,s);

                if (bs->value)
                    waddstr(janela1,"[0 >1]");
                else
                    waddstr(janela1,"[0< 1]");
            }
            break;

        case LED:
            {
                indicator* out = obja[i].objptr;
                if (out->value)
                    waddstr(janela1,"[#]");
                else
                    waddstr(janela1,"[.]");
            }
            break;

        case XDIGIT:
            {
                indicator* out = obja[i].objptr;
                char s[10];
                sprintf(s,"%X",out->value & 0x0F);
                waddstr(janela1,s);
            }
            break;
        }

        if (obja[i].name){
            wmove(janela1,1 + obja[i].pos_h,obja[i].pos_w);
            waddstr(janela1,obja[i].name);
        }
    }

    wrefresh(janela1);
    wrefresh(janela0);

    pthread_mutex_unlock(&ncursesmutex);
}

////////////////////////////////////////////////////////////////////////////////
void *refresh_thread(void *args){

    refresh_run = 1;

    while (refresh_run){

        if (must_refresh)
            board_refresh();

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


pthread_mutex_t transitionmutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t clkthread;

int clock_run = 0;
int clock_pausing = 0;
int clock_state_paused = 0;
int clock_period_us = 500000;
int iclk = 1;

#define NCLKS 9

int CLKS_PERIOD_US[NCLKS] = {1000000,    //1s
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

////////////////////////////////////////////////////////////////////////////////
void *clock_thread(void *args){

    clock_period_us = CLKS_PERIOD_US[iclk];
    clock_run = 1;

    clock_redraw();

    while (clock_run){

        if (switch_to_clock){

            if (clock_pausing){

                pthread_mutex_lock(&transitionmutex);
                bitswitch_setval(switch_to_clock, 0);
                pthread_mutex_unlock(&transitionmutex);
                clock_state_paused = 1;
                usleep(100000);
                continue;
            }
            else
                clock_state_paused = 0;

            pthread_mutex_lock(&transitionmutex);
            bitswitch_setval(switch_to_clock, 1);
            pthread_mutex_unlock(&transitionmutex);

            board_set_refresh();
            usleep(clock_period_us/2);

            pthread_mutex_lock(&transitionmutex);
            bitswitch_setval(switch_to_clock, 0);
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

        pthread_mutex_lock(&transitionmutex);
        bitswitch_setval(switch_to_clock, 1);
        bitswitch_setval(switch_to_clock, 0);
        pthread_mutex_unlock(&transitionmutex);
        board_set_refresh();
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

////////////////////////////////////////////////////////////////////////////////
int board_init(int width, int height){

    board_w = width;
    board_h = height;
    nobjs = 0;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int board_add_manual_switch(bitswitch *bs, int pos_w, int pos_h, int key, char *name){

/*
  F1[0 >1]
  UP/DOWN

  F1[0< 1]
  UP/DOWN
*/

    if (nobjs >= NOBJ)
        return -1;

    obja[nobjs].pos_w  = pos_w;
    obja[nobjs].pos_h  = pos_h;
    obja[nobjs].type   = MANUAL_SWITCH;
    obja[nobjs].objptr = bs;
    obja[nobjs].key    = key;
    obja[nobjs].name   = name;
    nobjs++;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int board_add_led(indicator *out, int pos_w, int pos_h, char *name){

/*
  [.]
  STATUS

  [#]
  STATUS
*/

    if (nobjs >= NOBJ)
        return -1;

    obja[nobjs].pos_w  = pos_w;
    obja[nobjs].pos_h  = pos_h;
    obja[nobjs].type   = LED;
    obja[nobjs].objptr = out;
    obja[nobjs].key    = 0;
    obja[nobjs].name   = name;
    nobjs++;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int board_add_xdigit(indicator *out, int pos_w, int pos_h, char *name){

/*
  A
  STATUS
*/

    if (nobjs >= NOBJ)
        return -1;

    obja[nobjs].pos_w  = pos_w;
    obja[nobjs].pos_h  = pos_h;
    obja[nobjs].type   = XDIGIT;
    obja[nobjs].objptr = out;
    obja[nobjs].key    = 0;
    obja[nobjs].name   = name;
    nobjs++;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int board_assign_clock_to_switch(bitswitch *bs){

    switch_to_clock = bs;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define LINHAS_JANELA2 8
#define LINHAS_JANELA2B (LINHAS_JANELA2+2)

void board_run(){

    signal (SIGHUP,SIG_IGN);
    signal (SIGINT,SIG_IGN);
    signal (SIGTERM,sigterm_handler);
    signal (SIGTSTP,SIG_IGN);
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
    janela1 = newwin(TERM_LINES-1-LINHAS_JANELA2B,TERM_COLS-2,1,1);
    janela2 = newwin(LINHAS_JANELA2B,TERM_COLS,TERM_LINES-LINHAS_JANELA2B,0);
    janela3 = newwin(LINHAS_JANELA2,TERM_COLS-2,1+TERM_LINES-LINHAS_JANELA2B,1);

    //printf("4\n");
    desenha_janelas();

    //printf("5\n");

    keypad(janela1,TRUE);

    int stoprun = 0;

    pthread_create(&refthread, NULL, refresh_thread, NULL);
    pthread_create(&clkthread, NULL, clock_thread, NULL);

    board_set_refresh();

    while (!stoprun){

        restart_handlers();

        if (received_key()){

            int key = read_key();

            switch(key){

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

            int i;
            for (i = 0; i < nobjs; i++)
                if (obja[i].type == MANUAL_SWITCH){

                    if (obja[i].key == key){

                        pthread_mutex_lock(&transitionmutex);
                        bitswitch *bs = obja[i].objptr;
                        bitswitch_setval(bs, 1 ^ bs->value);
                        pthread_mutex_unlock(&transitionmutex);

                        board_set_refresh();
                    }
                }
        }

    }

    refresh_thread_stop();
    clock_thread_stop();
    pthread_mutex_destroy(&transitionmutex);

    endwin();
}
