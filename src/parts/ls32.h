/*
 * ls32.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS32_H_
#define LS32_H_

#include "update.h"

#define NUM_ELEM_LS32 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[NUM_ELEM_LS32];
    vallist *ina_rootptr[NUM_ELEM_LS32];
    bitvalue_t inb[NUM_ELEM_LS32];
    vallist *inb_rootptr[NUM_ELEM_LS32];
    bitvalue_t oldvalue[NUM_ELEM_LS32];
    bitvalue_t value[NUM_ELEM_LS32];
    ehandler *out_event_handler_root[NUM_ELEM_LS32];
} ls32;

ls32 *ls32_create();

void ls32_destroy (ls32 **dest);

void ls32_connect_y1(ls32 *source, void *dest, event_function_t dest_event_handler);
void ls32_connect_y2(ls32 *source, void *dest, event_function_t dest_event_handler);
void ls32_connect_y3(ls32 *source, void *dest, event_function_t dest_event_handler);
void ls32_connect_y4(ls32 *source, void *dest, event_function_t dest_event_handler);

void ls32_in_a1(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_b1(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_a2(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_b2(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_a3(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_b3(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_a4(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls32_in_b4(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS32_H_ */
