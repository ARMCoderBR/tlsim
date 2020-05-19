////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS157_H_
#define LS157_H_

#include "update.h"

#define NUM_ELEM_LS157 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[NUM_ELEM_LS157];
    vallist *ina_rootptr[NUM_ELEM_LS157];
    bitvalue_t inb[NUM_ELEM_LS157];
    vallist *inb_rootptr[NUM_ELEM_LS157];
    bitvalue_t in_sel;
    vallist *in_sel_rootptr;
    bitvalue_t in_g;
    vallist *in_g_rootptr;
    bitvalue_t oldvalue[NUM_ELEM_LS157];
    bitvalue_t value[NUM_ELEM_LS157];
    ehandler *out_event_handler_root[NUM_ELEM_LS157];
    event_context_t *ec;
} ls157;

ls157 *ls157_create(event_context_t *ec);

void ls157_destroy(ls157 **dest);

void ls157_connect_y1(ls157 *source, void *dest, event_function_t dest_event_handler);
void ls157_connect_y2(ls157 *source, void *dest, event_function_t dest_event_handler);
void ls157_connect_y3(ls157 *source, void *dest, event_function_t dest_event_handler);
void ls157_connect_y4(ls157 *source, void *dest, event_function_t dest_event_handler);

void ls157_in_a1(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_b1(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_a2(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_b2(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_a3(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_b3(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_a4(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls157_in_b4(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls157_in_sel(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls157_in_g(ls157 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS157_H_ */
