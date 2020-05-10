////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef PROGCTR_H_
#define PROGCTR_H_

// Based on Ben Eater's project (https://www.youtube.com/watch?v=tNwU7pK_3tk)

#include "ls161.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

typedef struct {

    part_destroy_function_t destroy;
    ls161 *ls161;
    ls245 *ls245_1;
    indicator *led[4];
    indicator *ledclk;
    char name[30];

} progctr;

progctr *progctr_create(char *name);

void progctr_destroy(progctr **dest);

void progctr_connect_bit_out (progctr *source, int index, void *dest, event_function_t dest_event_handler);

void progctr_in_data_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from,progctr *dest,int index);

///////////////////////////////
void progctr_in_clock(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

void progctr_in_load(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

void progctr_in_clear(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

void progctr_in_ct_enable(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

void progctr_in_oenable(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

void progctr_in_data0(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);
void progctr_in_data1(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);
void progctr_in_data2(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);
void progctr_in_data3(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp);

extern void (*progctr_in_dataN[])(void *dest, bitvalue_t *valptr, timevalue_t timestamp);

board_object *progctr_board_create(progctr *reg, int key, char *name); // Requer NCURSES

#endif /* REG_8BIT_H_ */
