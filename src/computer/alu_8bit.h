/*
 * alu_8bit.h
 *
 *  Created on: 18 de abr de 2020
 *      Author: milton
 */

#ifndef ALU_8BIT_H_
#define ALU_8BIT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=S-3fXU3FZQc)

#include "ls86.h"
#include "ls283.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    ls86 *ls86_hi, *ls86_lo;
    ls283 *ls283_hi, *ls283_lo;
    ls245 *ls245_1;
    indicator *led[8];
    char name[30];

} alu_8bit;

alu_8bit *alu_8bit_create(char *name);

void alu_8bit_connect_bit_out (alu_8bit *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void alu_8bit_in_dataA_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,alu_8bit *dest,int index);

void alu_8bit_in_dataB_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,alu_8bit *dest,int index);

void alu_8bit_in_sub_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, alu_8bit *dest);

void alu_8bit_in_enable_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, alu_8bit *dest);

///////////////////////////////

extern void (*alu_8bit_in_dataAN[])(void *dest, int *valptr, int timestamp);
extern void (*alu_8bit_in_dataBN[])(void *dest, int *valptr, int timestamp);

board_object *alu_8bit_board_create(alu_8bit *alu, int key, char *name); // Requer NCURSES

#endif /* ALU_8BIT_H_ */
