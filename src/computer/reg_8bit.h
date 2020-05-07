/*
 * reg_8bit.h
 *
 *  Created on: 6 de abr de 2020
 *      Author: milton
 */

#ifndef REG_8BIT_H_
#define REG_8BIT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "ls173.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef enum{

    REG8BIT_NORMAL,
    REG8BIT_IR

} reg8bit_type_t;

typedef struct {

    ls173 *ls173_hi, *ls173_lo;
    ls245 *ls245_1;
    indicator *led[8];
    indicator *ledclk;
    reg8bit_type_t type;
    char name[30];

} reg_8bit;

reg_8bit *reg_8bit_create(reg8bit_type_t type, char *name);

void reg_8bit_destroy(reg_8bit **dest);

void reg_8bit_connect_bit_out (reg_8bit *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void reg_8bit_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,reg_8bit *dest,int index);

void reg_8bit_in_load_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,reg_8bit *dest);

void reg_8bit_in_clear_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_8bit *dest);

void reg_8bit_in_enable_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_8bit *dest);

void reg_8bit_in_clock_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_8bit *dest);

///////////////////////////////
void reg_8bit_in_enable(reg_8bit *dest, int *valptr, int timestamp);

void reg_8bit_in_clock(reg_8bit *dest, int *valptr, int timestamp);


extern void (*reg_8bit_in_dataN[])(void *dest, int *valptr, int timestamp);


void reg_8bit_in_data0(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data1(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data2(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data3(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data4(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data5(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data6(reg_8bit *dest, int *valptr, int timestamp);
void reg_8bit_in_data7(reg_8bit *dest, int *valptr, int timestamp);



board_object *reg_8bit_board_create(reg_8bit *reg, int key, char *name); // Requer NCURSES

#endif /* REG_8BIT_H_ */
