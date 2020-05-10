////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS86_H_
#define LS86_H_

#include "update.h"

#define NUM_ELEM_LS86 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[NUM_ELEM_LS86];
    vallist *ina_rootptr[NUM_ELEM_LS86];
    bitvalue_t inb[NUM_ELEM_LS86];
    vallist *inb_rootptr[NUM_ELEM_LS86];
    bitvalue_t oldvalue[NUM_ELEM_LS86];
    bitvalue_t value[NUM_ELEM_LS86];
    ehandler *out_event_handler_root[NUM_ELEM_LS86];
} ls86;

ls86 *ls86_create();

void ls86_destroy (ls86 **dest);

void ls86_connect_y1(ls86 *source, void *dest, event_function_t dest_event_handler);
void ls86_connect_y2(ls86 *source, void *dest, event_function_t dest_event_handler);
void ls86_connect_y3(ls86 *source, void *dest, event_function_t dest_event_handler);
void ls86_connect_y4(ls86 *source, void *dest, event_function_t dest_event_handler);

void ls86_in_a1(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_b1(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_a2(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_b2(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_a3(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_b3(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_a4(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls86_in_b4(ls86 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS86_H_ */
