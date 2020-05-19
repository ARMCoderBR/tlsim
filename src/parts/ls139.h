////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS139_H_
#define LS139_H_

#include "update.h"

#define NUM_ELEM_LS139 2

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[NUM_ELEM_LS139];
    vallist *ina_rootptr[NUM_ELEM_LS139];
    bitvalue_t inb[NUM_ELEM_LS139];
    vallist *inb_rootptr[NUM_ELEM_LS139];
    bitvalue_t ing[NUM_ELEM_LS139];
    vallist *ing_rootptr[NUM_ELEM_LS139];
    bitvalue_t y0[NUM_ELEM_LS139];
    bitvalue_t y1[NUM_ELEM_LS139];
    bitvalue_t y2[NUM_ELEM_LS139];
    bitvalue_t y3[NUM_ELEM_LS139];
    bitvalue_t oy0[NUM_ELEM_LS139];
    bitvalue_t oy1[NUM_ELEM_LS139];
    bitvalue_t oy2[NUM_ELEM_LS139];
    bitvalue_t oy3[NUM_ELEM_LS139];
    ehandler *y0_event_handler_root[NUM_ELEM_LS139];
    ehandler *y1_event_handler_root[NUM_ELEM_LS139];
    ehandler *y2_event_handler_root[NUM_ELEM_LS139];
    ehandler *y3_event_handler_root[NUM_ELEM_LS139];
    event_context_t *ec;
} ls139;

ls139 *ls139_create(event_context_t *ec);

void ls139_destroy(ls139 **dest);

void ls139_connect_1y0(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_1y1(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_1y2(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_1y3(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_2y0(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_2y1(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_2y2(ls139 *source, void *dest, event_function_t dest_event_handler);
void ls139_connect_2y3(ls139 *source, void *dest, event_function_t dest_event_handler);

void ls139_in_1a(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls139_in_1b(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls139_in_1g(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls139_in_2a(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls139_in_2b(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls139_in_2g(ls139 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS139_H_ */
