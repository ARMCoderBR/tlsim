/*
 * reg_out.h
 *
 *  Created on: 01 de mai de 2020
 *      Author: milton
 */

#ifndef REG_OUT_H_
#define REG_OUT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "ls173.h"
#include "board.h"
#include "at28c16.h"
#include "clkgen.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct {

    ls173 *ls173_hi, *ls173_lo;
    at28c16 *eep1;
    dis7seg *display[4];
    indicator *led[8];
    indicator *ledclk;
    clkgen *clk;
    indicator *ledclki;
    char name[30];

} reg_out;

reg_out *reg_out_create(char *name);

void reg_out_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,reg_out *dest,int index);

void reg_out_in_load_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,reg_out *dest);

void reg_out_in_clear_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest);

void reg_out_in_clock_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest);

///////////////////////////////
void reg_out_in_clock(reg_out *dest, int *valptr, int timestamp);


extern void (*reg_out_in_dataN[])(void *dest, int *valptr, int timestamp);


void reg_out_in_data0(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data1(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data2(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data3(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data4(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data5(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data6(reg_out *dest, int *valptr, int timestamp);
void reg_out_in_data7(reg_out *dest, int *valptr, int timestamp);


board_object *reg_out_board_create(reg_out *reg, int key, char *name); // Requer NCURSES

#endif /* REG_OUT_H_ */
