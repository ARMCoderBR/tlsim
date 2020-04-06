/*
 * ls02.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS02_H_
#define LS02_H_

#include "update.h"

#define NUM_ELEM_LS02 4

typedef struct {

    int ina[NUM_ELEM_LS02];
    vallist *ina_rootptr[NUM_ELEM_LS02];
    int inb[NUM_ELEM_LS02];
    vallist *inb_rootptr[NUM_ELEM_LS02];
    int oldvalue[NUM_ELEM_LS02];
    int value[NUM_ELEM_LS02];
    ehandler *out_event_handler_root[NUM_ELEM_LS02];
} ls02;

ls02 *ls02_create();

void ls02_connect_y1(ls02 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls02_connect_y2(ls02 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls02_connect_y3(ls02 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls02_connect_y4(ls02 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls02_in_a1(ls02 *dest, int *valptr, int timestamp);
void ls02_in_b1(ls02 *dest, int *valptr, int timestamp);
void ls02_in_a2(ls02 *dest, int *valptr, int timestamp);
void ls02_in_b2(ls02 *dest, int *valptr, int timestamp);
void ls02_in_a3(ls02 *dest, int *valptr, int timestamp);
void ls02_in_b3(ls02 *dest, int *valptr, int timestamp);
void ls02_in_a4(ls02 *dest, int *valptr, int timestamp);
void ls02_in_b4(ls02 *dest, int *valptr, int timestamp);

#endif /* LS02_H_ */
