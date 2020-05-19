////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS138_H_
#define LS138_H_

#include "update.h"

#define NUM_BITS_LS138 8

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina;
    vallist *ina_rootptr;
    bitvalue_t inb;
    vallist *inb_rootptr;
    bitvalue_t inc;
    vallist *inc_rootptr;
    bitvalue_t ing2a;
    vallist *ing2a_rootptr;
    bitvalue_t ing2b;
    vallist *ing2b_rootptr;
    bitvalue_t ing1;
    vallist *ing1_rootptr;
    bitvalue_t y[NUM_BITS_LS138];
    bitvalue_t oy[NUM_BITS_LS138];
    ehandler *y_event_handler_root[NUM_BITS_LS138];
    event_context_t *ec;
} ls138;

ls138 *ls138_create(event_context_t *ec);

void ls138_destroy(ls138 **dest);

void ls138_connect_y0(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y1(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y2(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y3(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y4(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y5(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y6(ls138 *source, void *dest, event_function_t dest_event_handler);
void ls138_connect_y7(ls138 *source, void *dest, event_function_t dest_event_handler);

void ls138_ina(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls138_inb(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls138_inc(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls138_ing2a(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls138_ing2b(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls138_ing1(ls138 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS138_H_ */
