/*
 * ls04.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS04_H_
#define LS04_H_

#include "update.h"

#define NUM_ELEM_LS04 6

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t in[NUM_ELEM_LS04];
    vallist *in_rootptr[NUM_ELEM_LS04];
    bitvalue_t oldvalue[NUM_ELEM_LS04];
    bitvalue_t value[NUM_ELEM_LS04];
    ehandler *out_event_handler_root[NUM_ELEM_LS04];
} ls04;

ls04 *ls04_create();

void ls04_destroy (ls04 **dest);

void ls04_connect_y1(ls04 *source, void *dest, event_function_t dest_event_handler);
void ls04_connect_y2(ls04 *source, void *dest, event_function_t dest_event_handler);
void ls04_connect_y3(ls04 *source, void *dest, event_function_t dest_event_handler);
void ls04_connect_y4(ls04 *source, void *dest, event_function_t dest_event_handler);
void ls04_connect_y5(ls04 *source, void *dest, event_function_t dest_event_handler);
void ls04_connect_y6(ls04 *source, void *dest, event_function_t dest_event_handler);

void ls04_in_a1(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls04_in_a2(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls04_in_a3(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls04_in_a4(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls04_in_a5(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls04_in_a6(ls04 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS04_H_ */
