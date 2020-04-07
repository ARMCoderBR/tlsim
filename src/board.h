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

typedef enum {

    MANUAL_SWITCH,
    LED,
    XDIGIT,
    BOARD,
} control_type;

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
    void/*board_object*/ *objptr_root;
    void/*board_object*/ *objptr_next;
    void/*board_object*/ *board_on_focus;
} board_object;

int board_add_manual_switch(board_object *b, bitswitch *bs, int pos_w, int pow_h, int key, char *name);

int board_add_led(board_object *b, indicator *out, int pos_w, int pos_h, char *name);

int board_add_xdigit(board_object *b, indicator *out, int pos_w, int pos_h, char *name);

int board_add_board(board_object *b, board_object *board, int pos_w, int pos_h);

int board_assign_clock_to_switch(bitswitch *bs);

int board_run(board_object *board);

board_object *board_create(int width, int height, int key, char *name);

board_object *mainboard_create(char *name);



#endif /* BOARD_H_ */
