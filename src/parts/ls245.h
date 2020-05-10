/*
 * ls245.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS245_H_
#define LS245_H_

#include "update.h"

#define NUM_ELEM_LS245 8

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t inpa[NUM_ELEM_LS245];
    vallist *inpa_rootptr[NUM_ELEM_LS245];
    bitvalue_t inpb[NUM_ELEM_LS245];
    vallist *inpb_rootptr[NUM_ELEM_LS245];
    bitvalue_t outa_o[NUM_ELEM_LS245];
    bitvalue_t outa[NUM_ELEM_LS245];
    bitvalue_t outb_o[NUM_ELEM_LS245];
    bitvalue_t outb[NUM_ELEM_LS245];
    bitvalue_t in_dir;
    vallist *in_dir_rootptr;
    bitvalue_t in_oe;
    vallist *in_oe_rootptr;
    ehandler *outq_event_handler_root[2*NUM_ELEM_LS245];
} ls245;

ls245 *ls245_create();

void ls245_destroy(ls245 **dest);

void ls245_connect_a1(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a2(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a3(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a4(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a5(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a6(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a7(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_a8(ls245 *source, void *dest, event_function_t dest_event_handler);

void ls245_connect_b1(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b2(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b3(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b4(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b5(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b6(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b7(ls245 *source, void *dest, event_function_t dest_event_handler);
void ls245_connect_b8(ls245 *source, void *dest, event_function_t dest_event_handler);

void ls245_in_a1(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a2(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a3(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a4(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a5(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a6(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a7(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_a8(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls245_in_b1(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b2(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b3(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b4(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b5(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b6(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b7(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_b8(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls245_in_dir(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls245_in_oe(ls245 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS374_H_ */
