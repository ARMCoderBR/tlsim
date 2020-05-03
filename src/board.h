/*
 * interactive.h
 *
 *  Created on: 28 de mar de 2020
 *      Author: milton
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "bitswitch.h"
#include "indicator.h"
#include "dis7seg.h"

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

#define NAMESIZE 32

typedef struct {

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

int board_add_manual_switch(board_object *b, bitswitch *bs, int pos_w, int pow_h, int key, char *name);

int board_add_led(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_display_7seg(board_object *b, dis7seg *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_xdigit(board_object *b, indicator *out, int pos_w, int pos_h, char *name, led_color_t color);

int board_add_board(board_object *b, board_object *board, int pos_w, int pos_h);

int board_run(board_object *board);

board_object *board_create(int width, int height, int key, char *name);

board_object *mainboard_create(char *name);

void board_clock_connect(void *objdest, void (*objdest_event_handler)(void *objdest, int *valptr, int timestamp));

void board_mutex_lock();

void board_mutex_unlock();

#endif /* BOARD_H_ */
