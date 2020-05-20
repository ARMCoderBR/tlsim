////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "update.h"
#include "board.h"
#include "bitswitch.h"
#include "update.h"
#include "dis7seg.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define LINHAS_JANELA2 3
#define LINHAS_JANELA2B (LINHAS_JANELA2+2)

int TERM_LINES = 0;
int TERM_COLS = 0;
fd_set readfds;

WINDOW *janela0;
WINDOW *janela1;
WINDOW *janela2;
WINDOW *janela3;

pthread_mutex_t transitionmutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t setrefmutex = PTHREAD_MUTEX_INITIALIZER;
bool_t reader_ok = 0;

pthread_mutex_t ncursesmutex = PTHREAD_MUTEX_INITIALIZER;

void board_mutex_lock(){

    pthread_mutex_lock(&transitionmutex);
}

void board_mutex_unlock(){

    pthread_mutex_unlock(&transitionmutex);
}

////////////////////////////////////////////////////////////////////////////////
void combine_7seg(int segmap, int C[]){

    int i;
    for (i = 0; i < 10; i++)
        C[i] = ' ';

/*
   +----+----+----+----+
   |                   |
   | C0   C1   C2      |
   |                   |
   +----+----+----+----+
   |                   |
   | C3   C4   C5      |
   |                   |
   +----+----+----+----+
   |                   |
   | C6   C7   C8   C9 |
   |                   |
   +----+----+----+----+

   A F    C0
   0 0    SPACE
   0 1    ACS_ULCORNER
   1 0    ACS_HLINE
   1 1    ACS_ULCORNER
*/

    switch(segmap & (MSK_A|MSK_F)){

    case MSK_F:
        C[0] = ACS_URCORNER;
        break;
    case MSK_A|MSK_F:
        C[0] = ACS_ULCORNER;
        break;
    case MSK_A:
        C[0] = ACS_HLINE;
        break;
    }

/*
   A      C1
   0      SPACE
   1      ACS_HLINE
*/

    if (segmap & MSK_A)
        C[1] = ACS_HLINE;

/*
   A B    C2
   0 0    SPACE
   0 1    ACS_URCORNER
   1 0    ACS_HLINE
   1 1    ACS_URCORNER
*/

    switch(segmap & (MSK_A|MSK_B)){

    case MSK_B:
    case MSK_A|MSK_B:
        C[2] = ACS_URCORNER;
        break;
    case MSK_A:
        C[2] = ACS_HLINE;
        break;
    }

/*
   E F G  C3
   0 0 0  SPACE
   0 0 1  ACS_HLINE
   0 1 0  ACS_LLCORNER
   0 1 1  ACS_LLCORNER
   1 0 0  ACS_ULCORNER
   1 0 1  ACS_ULCORNER
   1 1 0  ACS_VLINE
   1 1 1  ACS_LTEE
*/

    switch(segmap & (MSK_E|MSK_F|MSK_G)){

    case MSK_F:
    case MSK_F|MSK_G:
        C[3] = ACS_LLCORNER;
        break;
    case MSK_E:
    case MSK_E|MSK_G:
        C[3] = ACS_ULCORNER;
        break;
    case MSK_G:
        C[3] = ACS_HLINE;
        break;
    case MSK_E|MSK_F:
        C[3] = ACS_VLINE;
        break;
    case MSK_E|MSK_F|MSK_G:
        C[3] = ACS_LTEE;
        break;
    }

/*
   G      C4
   0      SPACE
   1      ACS_HLINE
*/

    if (segmap & MSK_G)
        C[4] = ACS_HLINE;

/*
   B C G  C5
   0 0 0  SPACE
   0 0 1  ACS_HLINE
   0 1 0  ACS_URCORNER
   0 1 1  ACS_URCORNER
   1 0 0  ACS_LRCORNER
   1 0 1  ACS_LRCORNER
   1 1 0  ACS_VLINE
   1 1 1  ACS_RTEE
*/

    switch(segmap & (MSK_B|MSK_C|MSK_G)){

    case MSK_G:
        C[5] = ACS_HLINE;
        break;
    case MSK_C:
    case MSK_C|MSK_G:
        C[5] = ACS_URCORNER;
        break;
    case MSK_B:
    case MSK_B|MSK_G:
        C[5] = ACS_LRCORNER;
        break;
    case MSK_B|MSK_C:
        C[5] = ACS_VLINE;
        break;
    case MSK_B|MSK_C|MSK_G:
        C[5] = ACS_RTEE;
        break;
    }

/*
   D E    C6
   0 0    SPACE
   0 1    ACS_LLCORNER
   1 0    ACS_HLINE
   1 1    ACS_LLCORNER
*/

    switch(segmap & (MSK_D|MSK_E)){

    case MSK_E:
    case MSK_D|MSK_E:
        C[6] = ACS_LLCORNER;
        break;
    case MSK_D:
        C[6] = ACS_HLINE;
        break;
    }

/*
   D      C7
   0      SPACE
   1      ACS_HLINE
*/

    if (segmap & MSK_D)
        C[7] = ACS_HLINE;

/*
   C D    C8
   0 0    SPACE
   0 1    ACS_HLINE
   1 0    ACS_LRCORNER
   1 1    ACS_LRCORNER
*/

    switch(segmap & (MSK_C|MSK_D)){

    case MSK_C:
        C[8] = ACS_LLCORNER;
        break;
    case MSK_C|MSK_D:
        C[8] = ACS_LRCORNER;
        break;
    case MSK_D:
        C[8] = ACS_HLINE;
        break;
    }

/*
   DP     C9
   0      SPACE
   1      o
*/

    if (segmap & MSK_DP)
        C[9] = ACS_DIAMOND;//'o';
}

////////////////////////////////////////////////////////////////////////////////
void display_7seg(WINDOW *wnd, int segmap, int common, int pos_w, int pos_h){

/*
       A
    +-----+
   F|     |B
    +--G--+
   E|     |C
    +-----+
       D
*/

    int C[10];

    if (!common)
        segmap = 0;

    combine_7seg(segmap, C);

    wattron(wnd,COLOR_PAIR(LED_RED));

    wmove(wnd, pos_h, pos_w);
    //waddch(janela1,' ');
    waddch(wnd,C[0]);
    waddch(wnd,C[1]);
    waddch(wnd,C[1]);
    waddch(wnd,C[2]);
    waddch(wnd,' ');

    wmove(wnd, pos_h+1, pos_w);
    //waddch(janela1,' ');
    waddch(wnd,C[3]);
    waddch(wnd,C[4]);
    waddch(wnd,C[4]);
    waddch(wnd,C[5]);
    waddch(wnd,' ');

    wmove(wnd, pos_h+2, pos_w);
    //waddch(janela1,' ');
    waddch(wnd,C[6]);
    waddch(wnd,C[7]);
    waddch(wnd,C[7]);
    waddch(wnd,C[8]);
    waddch(wnd,C[9]);

    wattroff(wnd,COLOR_PAIR(LED_WHITE));
}

////////////////////////////////////////////////////////////////////////////////
void desenha_janelas(void)
{
    wbkgd(janela0,COLOR_PAIR(10));
    wbkgd(janela1,COLOR_PAIR(10));
    wbkgd(janela2,COLOR_PAIR(10));
    wbkgd(janela3,COLOR_PAIR(10));

    /*box(janela0, 0 , 0);  */      /* 0, 0 gives default characters
                                 * for the vertical and horizontal
                                 * lines            */
    wrefresh(janela0);

    wclear(janela1);
    wrefresh(janela1);

    box(janela2, 0 , 0);        /* 0, 0 gives default characters
                                 * for the vertical and horizontal
                                 * lines            */
    wmove(janela1, TERM_LINES-LINHAS_JANELA2B-2, 1);
    waddstr(janela1,"F2,F3: Change focused Panel     https://github.com/ARMCoderBR/tlsim");

    wrefresh(janela2);
}

////////////////////////////////////////////////////////////////////////////////
void restart_handlers(void)
{
    struct timeval tv;

    FD_ZERO (&readfds);
    FD_SET(0,&readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 100;    // 100 us

    select (1,&readfds,NULL,NULL,&tv);
}

////////////////////////////////////////////////////////////////////////////////
int received_key(void)
{
    return (FD_ISSET(0,&readfds));
}

////////////////////////////////////////////////////////////////////////////////
int read_key(void)
{
    pthread_mutex_lock(&ncursesmutex);
    int key = wgetch(janela1);
    pthread_mutex_unlock(&ncursesmutex);
    return key;
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

#include "clkgen.h"

clkgen *boardclk = NULL;

void board_set_clk(clkgen *clk){

    boardclk = clk;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

pthread_t refthread;
bool_t refresh_run = 0;
int piperefresh[2];

////////////////////////////////////////////////////////////////////////////////
void board_set_refresh(){

    if (!reader_ok) return;

    pthread_mutex_lock(&setrefmutex);

    char buf[] = "1";
    write(piperefresh[1],buf,1);

    pthread_mutex_unlock(&setrefmutex);
}

////////////////////////////////////////////////////////////////////////////////
void rectangle(WINDOW *wnd, int y1, int x1, int y2, int x2)
{
    mvwhline(wnd, y1, x1, 0, x2-x1);
    mvwhline(wnd, y2, x1, 0, x2-x1);
    mvwvline(wnd, y1, x1, 0, y2-y1);
    mvwvline(wnd, y1, x2, 0, y2-y1);
    mvwaddch(wnd, y1, x1, ACS_ULCORNER);
    mvwaddch(wnd, y2, x1, ACS_LLCORNER);
    mvwaddch(wnd, y1, x2, ACS_URCORNER);
    mvwaddch(wnd, y2, x2, ACS_LRCORNER);
}

////////////////////////////////////////////////////////////////////////////////

#define MAX_FOCUSEABLES_BOARDS 50

bool_t focustable_done = 0;
int num_focuseable_boards = 0;
int current_board_on_focus = 0;
board_object *board_on_focus[MAX_FOCUSEABLES_BOARDS];

////////////////////////////////////////////////////////////////////////////////
void board_refresh_a(board_object *b, int new_h, int new_w){

    if (b->type != BOARD) return;   // Erro interno - nunca deve acontecer.

    wattrset(janela1,A_NORMAL);
    rectangle(janela1, new_h, new_w, new_h+b->w_height-1, new_w+b->w_width-1);

    if (b->name[0]){

        wmove(janela1, new_h, 1 + new_w);
        waddch(janela1,'[');
        waddstr(janela1,b->name);
        waddch(janela1,']');
    }

    int has_key = 0;
    board_object *thisboard = b;

    b = b->objptr_root;

    while (b){

        wmove(janela1, new_h + b->pos_h, new_w + b->pos_w);

        switch (b->type){

        case MANUAL_SWITCH:
            {
                bitswitch* bs = b->objptr;
                if (bs->value){
                    wattron(janela1,COLOR_PAIR(6));
                    waddstr(janela1,"[ "); waddch(janela1,ACS_DIAMOND); waddstr(janela1,"1]");
                    wattroff(janela1,COLOR_PAIR(6));
                }
                else{
                    wattron(janela1,COLOR_PAIR(LED_WHITE));
                    waddstr(janela1,"["); waddch(janela1,ACS_DIAMOND); waddstr(janela1," 1]");
                    wattroff(janela1,COLOR_PAIR(LED_WHITE));
                }
            }
            break;

        case LED:
            {
                indicator* out = b->objptr;
                wattron(janela1,COLOR_PAIR(b->color));
                if (out->value){
                    wattron(janela1,A_STANDOUT);
                    waddstr(janela1,"[#]");
                    wattroff(janela1,A_STANDOUT);
                }else
                    waddstr(janela1,"[.]");
                wattroff(janela1,COLOR_PAIR(b->color));
            }
            break;

        case DIS7SEG:
            {
                dis7seg *dis = b->objptr;
                display_7seg(janela1, dis->segmap, dis->common_val|(dis->count_persist?1:0), new_w + b->pos_w, new_h + b->pos_h);
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

        if ((b->type != BOARD)&&(b->type != DIS7SEG)){

            if (b->type == MANUAL_SWITCH){

                if (thisboard == board_on_focus[current_board_on_focus]){

                    wattrset(janela1,A_STANDOUT);
                }
            }
            wmove(janela1,1 + new_h + b->pos_h, new_w + b->pos_w);
            waddstr(janela1,b->name);
            wattrset(janela1,A_NORMAL);
        }

        if (b->type == MANUAL_SWITCH){

            has_key = 1;
            if (thisboard == board_on_focus[current_board_on_focus]){

                wattrset(janela1,A_STANDOUT);
            }

            int key = b->key;
            char s[10];
            if ((key >= KEY_F(1)) && (key <= KEY_F(12))){

                sprintf(s,"[F%d]",1+key-KEY_F(1));
            }
            else
                sprintf(s,"[%c]",key);

            waddstr(janela1,s);
            wattrset(janela1,A_NORMAL);
        }

        b = b->objptr_next;
    }

    if (!focustable_done)
        if (has_key)
            if (num_focuseable_boards < MAX_FOCUSEABLES_BOARDS)
                board_on_focus[num_focuseable_boards++] = thisboard;
}

////////////////////////////////////////////////////////////////////////////////
void board_refresh(board_object *b){

    pthread_mutex_lock(&ncursesmutex);

    board_refresh_a(b,0,0);

    focustable_done = 1;

    wrefresh(janela1);
    wrefresh(janela0);

    pthread_mutex_unlock(&ncursesmutex);
}

////////////////////////////////////////////////////////////////////////////////
void *refresh_thread(void *args){

    board_object * refboard = args;

    refresh_run = 1;
    char buf[16];

    fd_set rreadfds;
    struct timeval rtv;

    pipe(piperefresh);
    bool_t ref_pending = 0;

    struct timespec lastspec, nowspec;
    clock_gettime(CLOCK_REALTIME, &lastspec);

    reader_ok = 1;

    while (refresh_run){

        FD_ZERO(&rreadfds);
        FD_SET(piperefresh[0],&rreadfds);

        rtv.tv_sec = 0;
        rtv.tv_usec = 100000;

        select(1+piperefresh[0],&rreadfds,NULL,NULL,&rtv);

        if (FD_ISSET(piperefresh[0],&rreadfds)){

            read(piperefresh[0], buf, sizeof(buf));
            ref_pending = 1;
        }

        if (!ref_pending) continue;

        clock_gettime(CLOCK_REALTIME, &nowspec);

        int deltams = 1000 * (nowspec.tv_sec - lastspec.tv_sec);
        deltams += (nowspec.tv_nsec - lastspec.tv_nsec) / 1000000;

        if (deltams >= 40){

            board_refresh(refboard);
            lastspec = nowspec;
            ref_pending = 0;
        }
        else
            ref_pending = 1;
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void refresh_thread_stop(){

    refresh_run = 0;
    pthread_join(refthread,NULL);

    reader_ok = 0;

    close(piperefresh[0]);
    close(piperefresh[1]);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool_t clock_pausing = 0;
int iclk = 0;

#define NCLKS 10

const int CLKS_PERIOD_US[NCLKS] = {
                             2000000,    //2s
                             1000000,    //1s
                             500000,     //500ms
                             250000,     //250ms
                             100000,     //100ms
                             50000,      //50ms
                             25000,      //25ms
                             10000,      //10ms
                             5000,       //5ms
                             2000,       //2ms
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

    wmove(janela3,0,1);
    waddstr(janela3,s);

    wmove(janela3,1,1);
    if (!clock_pausing)
        waddstr(janela3,"RUNNING");
    else
        waddstr(janela3,"PAUSED ");

    wmove(janela3,2,1);
    waddstr(janela3,"F10:Pause/Pulse F11:Slower F12:Faster");

    wrefresh(janela3);

    pthread_mutex_unlock(&ncursesmutex);
}


////////////////////////////////////////////////////////////////////////////////
void clock_reinit(){

    int clock_period_us = CLKS_PERIOD_US[iclk];
    clkgen_set_us(boardclk, clock_period_us);
    clock_redraw();
}

////////////////////////////////////////////////////////////////////////////////
void clock_faster(){

    if (clock_pausing){

        clock_pausing = 0;
        clkgen_pause(boardclk, clock_pausing);
        clock_redraw();
        return;
    }

    if (iclk < (NCLKS-1)){

        iclk++;
        int clock_period_us = CLKS_PERIOD_US[iclk];
        clkgen_set_us(boardclk, clock_period_us);
        clock_redraw();
    }
}

////////////////////////////////////////////////////////////////////////////////
void clock_slower(){

    if (clock_pausing){

        clock_pausing = 0;
        clkgen_pause(boardclk, clock_pausing);
        clock_redraw();
        return;
    }

    if (iclk > 0){

        iclk--;
        int clock_period_us = CLKS_PERIOD_US[iclk];
        clkgen_set_us(boardclk, clock_period_us);
        clock_redraw();
    }
}

////////////////////////////////////////////////////////////////////////////////
void clock_pause(){

    if (!clock_pausing){

        clock_pausing = 1;
        clkgen_pause(boardclk, clock_pausing);
        clock_redraw();
        return;
    }
    else{

        clkgen_step(boardclk);
        usleep(10000);
    }

    clock_redraw();
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
    //b->board_on_focus = b;  // Focada nela própria no início.

    b->destroy = (void*)board_destroy;
    return b;
}

////////////////////////////////////////////////////////////////////////////////

//char stypes[5][20]= {
//
//    "MANUAL_SWITCH",
//    "LED",
//    "XDIGIT",
//    "BOARD",
//    "DIS7SEG"
//};

void board_destroy_a(board_object *dest){

//	printf("board_destroy()\n");
    if (dest == NULL) return;
    board_object *b = dest;
    if (b == NULL) return;

    if (b->type != BOARD) return;   // Erro interno - nunca deve acontecer.

    board_object *bo = b->objptr_root;
    while (bo){

        if (bo->type == BOARD){

        	//printf("will board_destroy()\n");

            board_destroy_a(bo);
        }else{

        	//printf("will object_destroy()\n");

        }

        board_object *tofree = bo;
        bo = bo->objptr_next;
        free(tofree);
    }
    b->objptr_root = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void board_destroy(board_object **dest){

	board_destroy_a(*dest);
    free(*dest);
    *dest = NULL;
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
int board_add_led(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color){

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

    out->refreshable = 1;
    obja->pos_w  = pos_w;
    obja->pos_h  = pos_h;
    obja->type   = LED;
    obja->color = color;
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
int board_add_display_7seg(board_object *b, dis7seg *out, int pos_w, int pos_h, char *name, led_color_t color){

    if (!b) return -2;
    if (!out) return -2;

    board_object *obja = malloc(sizeof(board_object));
    if (!obja) return -1;

    out->refreshable = 1;
    obja->pos_w  = pos_w;
    obja->pos_h  = pos_h;
    obja->type   = DIS7SEG;
    obja->color = color;
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
int board_add_xdigit(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color){

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

    board->pos_w = pos_w;
    board->pos_h = pos_h;
    return board_add_object(b, board);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int board_run(event_context_t *ec, board_object *board){

    bool_t resize = 0;

    if (!board) return -2;

    if (board->type != BOARD)
        return -10;

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

    start_color();
    init_pair(1, 8|COLOR_RED, 16|COLOR_BLACK);
    init_pair(2, 8|COLOR_GREEN, 16|COLOR_BLACK);
    init_pair(3, 8|COLOR_YELLOW, 16|COLOR_BLACK);
    init_pair(4, 8|COLOR_BLUE, 16|COLOR_BLACK);
    init_pair(5, COLOR_WHITE, 16|COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_RED);

    init_pair(10, COLOR_WHITE, 8|COLOR_BLACK);

    janela0 = newwin(TERM_LINES,TERM_COLS,0,0);
    janela1 = newwin(TERM_LINES-LINHAS_JANELA2B,TERM_COLS,0,0);
    janela2 = newwin(LINHAS_JANELA2B,TERM_COLS,TERM_LINES-LINHAS_JANELA2B,0);
    janela3 = newwin(LINHAS_JANELA2,TERM_COLS-2,1+TERM_LINES-LINHAS_JANELA2B,1);

    if (!board->w_width)
        board->w_width = TERM_COLS;

    if (!board->w_height)
        board->w_height = TERM_LINES-LINHAS_JANELA2B;

    desenha_janelas();

    clock_reinit();

    keypad(janela1,TRUE);

    bool_t stoprun = 0;

    pthread_create(&refthread, NULL, refresh_thread, board);

    board_set_refresh();

    while (!stoprun){

        board_mutex_lock();
        while (event_process(ec));
        board_mutex_unlock();

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
            clock_redraw();
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
                board_set_refresh();
                break;
            case KEY_F(2):
                if (num_focuseable_boards > 1){
                    if (current_board_on_focus)
                        current_board_on_focus--;
                    else
                        current_board_on_focus = num_focuseable_boards - 1;
                    board_set_refresh();
                }
                break;
            case KEY_F(3):
                if (num_focuseable_boards > 1){

                    current_board_on_focus = (current_board_on_focus+1) % num_focuseable_boards;
                    board_set_refresh();
                }
                break;
            case KEY_F(12):
                clock_faster();
                board_set_refresh();
                break;
            case KEY_F(11):
                clock_slower();
                board_set_refresh();
                break;
            case KEY_F(10):
                clock_pause();
                board_set_refresh();
                break;
            }

            board_object *pboardfocused = NULL;

            if (num_focuseable_boards)
                pboardfocused = board_on_focus[current_board_on_focus];

            board_object *p;

            if (pboardfocused)
                p = pboardfocused->objptr_root;
            else
                p = NULL;

            while(p){

                if (p->type == MANUAL_SWITCH){

                    if (p->key == key){

                        board_mutex_lock();
                        bitswitch *bs = p->objptr;
                        bitswitch_setval(bs, 1 ^ bs->value);
                        board_mutex_unlock();

                        board_set_refresh();
                    }
                }
                p = p->objptr_next;
            }
        }
    }

    refresh_thread_stop();

    pthread_mutex_destroy(&transitionmutex);

    pthread_mutex_destroy(&setrefmutex);

    endwin();

    return 0;
}

