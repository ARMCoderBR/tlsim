/*
 * interactive.h
 *
 *  Created on: 28 de mar de 2020
 *      Author: milton
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>

#include "update.h"
#include "bitswitch.h"
#include "indicator.h"
#include "dis7seg.h"
#include "clkgen.h"

typedef enum {

    MANUAL_SWITCH,
    LED,
    XDIGIT,
    BOARD,
    DIS7SEG
} control_type;

typedef enum {

    COLOR_NONE = 0,
    LED_RED = 1,
    LED_GREEN,
    LED_YELLOW,
    LED_BLUE,
    LED_WHITE
} led_color_t;

#define NAMESIZE 64

typedef struct {

    void (*destroy)(void **dest);
    int pos_w;
    int pos_h;
    control_type type;
    void *objptr;
    int key;
    char name[NAMESIZE];
    // Se objeto tipo BOARD, possui dimensões e pode conter múltiplos outros objetos.
    int w_width;
    int w_height;
    int color;
    void/*board_object*/ *objptr_root;
    void/*board_object*/ *objptr_next;
} board_object;

#define MAX_FOCUSEABLES_BOARDS 50
typedef struct {

    int TERM_LINES;
    int TERM_COLS;
    fd_set readfds;

    WINDOW *janela0;
    WINDOW *janela1;
    WINDOW *janela2;
    WINDOW *janela3;

    pthread_mutex_t setrefmutex;
    bool_t reader_ok;

    pthread_mutex_t ncursesmutex;

    clkgen *boardclk;
    pthread_t refthread;
    bool_t refresh_run;
    int piperefresh[2];

    bool_t focustable_done;
    int num_focuseable_boards;
    int current_board_on_focus;
    board_object *board_on_focus[MAX_FOCUSEABLES_BOARDS];

    bool_t clock_pausing;
    int iclk;

    board_object * board;
} board_ctx_t;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int board_add_manual_switch(board_object *b, bitswitch *bs, int pos_w, int pow_h, int key, char *name);

int board_add_led(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_display_7seg(board_object *b, dis7seg *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_xdigit(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_board(board_object *b, board_object *board, int pos_w, int pos_h);

int board_run(event_context_t *ec, board_object *board);

board_object *board_create(int width, int height, int key, char *name);

void board_destroy(board_object **dest);

board_object *mainboard_create(char *name);

void board_set_clk(clkgen *clk);

void board_set_refresh();

void board_initialize();

#endif /* BOARD_H_ */
