////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS08_H_
#define LS08_H_

#include "update.h"

#define NUM_ELEM_LS08 4

typedef struct {

    void (*destroy)(void **dest);
    bitvalue_t ina[NUM_ELEM_LS08];
    vallist *ina_rootptr[NUM_ELEM_LS08];
    bitvalue_t inb[NUM_ELEM_LS08];
    vallist *inb_rootptr[NUM_ELEM_LS08];
    bitvalue_t oldvalue[NUM_ELEM_LS08];
    bitvalue_t value[NUM_ELEM_LS08];
    ehandler *out_event_handler_root[NUM_ELEM_LS08];
    event_context_t *ec;
} ls08;

ls08 *ls08_create(event_context_t *ec);

void ls08_destroy (ls08 **dest);

void ls08_connect_y1(ls08 *source, void *dest, event_function_t dest_event_handler);
void ls08_connect_y2(ls08 *source, void *dest, event_function_t dest_event_handler);
void ls08_connect_y3(ls08 *source, void *dest, event_function_t dest_event_handler);
void ls08_connect_y4(ls08 *source, void *dest, event_function_t dest_event_handler);

void ls08_in_a1(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_b1(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_a2(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_b2(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_a3(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_b3(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_a4(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls08_in_b4(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS08_H_ */
