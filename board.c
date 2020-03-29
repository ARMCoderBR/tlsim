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

#include "board.h"
#include "bitswitch.h"

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

////////////////////////////////////////////////////////////////////////////////
void board_refresh(){

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
                output* out = obja[i].objptr;
                if (out->value)
                    waddstr(janela1,"[##]");
                else
                    waddstr(janela1,"[  ]");
            }
            break;

        case XDIGIT:
            {
                output* out = obja[i].objptr;
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
}

////////////////////////////////////////////////////////////////////////////////
void sigterm_handler(int sig){

    endwin();
    exit(0);
}

#define LINHAS_JANELA2 8
#define LINHAS_JANELA2B (LINHAS_JANELA2+2)

////////////////////////////////////////////////////////////////////////////////
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
    int presc = 0;
    int mainclk = 0;

    board_refresh();

    while (!stoprun){

        restart_handlers();

        if (received_key()){

            int key = read_key();

            switch(key){

            case 27:
                stoprun = 1;
                break;
            }

            int i;
            for (i = 0; i < nobjs; i++)
                if (obja[i].type == MANUAL_SWITCH){

                    if (obja[i].key == key){

                        bitswitch *bs = obja[i].objptr;
                        bitswitch_setval(bs, 1 ^ bs->value);
                        board_refresh();
                    }
                }
        }

        /////////////////////////////
        presc++;
        if (presc >= 5){

            presc = 0;
            if (switch_to_clock){

                bitswitch_setval(switch_to_clock, mainclk);
                board_refresh();
                mainclk ^= 1;
            }
        }
        /////////////////////////////
    }

    endwin();
}

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
int board_add_led(output *out, int pos_w, int pos_h, char *name){

/*
  [  ]
  STATUS

  [##]
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
int board_add_xdigit(output *out, int pos_w, int pos_h, char *name){

/*
  [  ]
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


