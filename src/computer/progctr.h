/*
 * progctr.h
 *
 *  Created on: 29 de abr de 2020
 *      Author: milton
 */

#ifndef PROGCTR_H_
#define PROGCTR_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=tNwU7pK_3tk)

#include "ls161.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

typedef struct {

    ls161 *ls161;
    ls245 *ls245_1;
    indicator *led[4];
    indicator *ledclk;
    char name[30];

} progctr;

progctr *progctr_create(char *name);

void progctr_connect_bit_out (progctr *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void progctr_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,progctr *dest,int index);

///////////////////////////////
void progctr_in_clock(progctr *dest, int *valptr, int timestamp);

void progctr_in_load(progctr *dest, int *valptr, int timestamp);

void progctr_in_clear(progctr *dest, int *valptr, int timestamp);

void progctr_in_eable(progctr *dest, int *valptr, int timestamp);

void progctr_in_data0(progctr *dest, int *valptr, int timestamp);
void progctr_in_data1(progctr *dest, int *valptr, int timestamp);
void progctr_in_data2(progctr *dest, int *valptr, int timestamp);
void progctr_in_data3(progctr *dest, int *valptr, int timestamp);

extern void (*progctr_in_dataN[])(void *dest, int *valptr, int timestamp);

board_object *progctr_board_create(progctr *reg, int key, char *name); // Requer NCURSES

#endif /* REG_8BIT_H_ */
