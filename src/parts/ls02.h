////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS02_H_
#define LS02_H_

#include "update.h"

#define NUM_ELEM_LS02 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[NUM_ELEM_LS02];
    vallist *ina_rootptr[NUM_ELEM_LS02];
    bitvalue_t inb[NUM_ELEM_LS02];
    vallist *inb_rootptr[NUM_ELEM_LS02];
    bitvalue_t oldvalue[NUM_ELEM_LS02];
    bitvalue_t value[NUM_ELEM_LS02];
    ehandler *out_event_handler_root[NUM_ELEM_LS02];
} ls02;

ls02 *ls02_create();

void ls02_destroy (ls02 **dest);

void ls02_connect_y1(ls02 *source, void *dest, event_function_t dest_event_handler);
void ls02_connect_y2(ls02 *source, void *dest, event_function_t dest_event_handler);
void ls02_connect_y3(ls02 *source, void *dest, event_function_t dest_event_handler);
void ls02_connect_y4(ls02 *source, void *dest, event_function_t dest_event_handler);

void ls02_in_a1(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_b1(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_a2(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_b2(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_a3(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_b3(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_a4(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls02_in_b4(ls02 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS02_H_ */
