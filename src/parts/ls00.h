/*
 * ls00.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS00_H_
#define LS00_H_

#include "update.h"

#define NUM_ELEM_LS00 4

typedef struct {

    int ina[NUM_ELEM_LS00];
    vallist *ina_rootptr[NUM_ELEM_LS00];
    int inb[NUM_ELEM_LS00];
    vallist *inb_rootptr[NUM_ELEM_LS00];
    int oldvalue[NUM_ELEM_LS00];
    int value[NUM_ELEM_LS00];
    ehandler *out_event_handler_root[NUM_ELEM_LS00];
} ls00;

ls00 *ls00_create();

void ls00_connect_y1(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls00_connect_y2(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls00_connect_y3(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls00_connect_y4(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls00_in_a1(ls00 *dest, int *valptr, int timestamp);
void ls00_in_b1(ls00 *dest, int *valptr, int timestamp);
void ls00_in_a2(ls00 *dest, int *valptr, int timestamp);
void ls00_in_b2(ls00 *dest, int *valptr, int timestamp);
void ls00_in_a3(ls00 *dest, int *valptr, int timestamp);
void ls00_in_b3(ls00 *dest, int *valptr, int timestamp);
void ls00_in_a4(ls00 *dest, int *valptr, int timestamp);
void ls00_in_b4(ls00 *dest, int *valptr, int timestamp);

#endif /* LS00_H_ */
