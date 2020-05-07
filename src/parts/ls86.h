/*
 * ls86.h
 *
 *  Created on: 18 de abr de 2020
 *      Author: milton
 */

#ifndef LS86_H_
#define LS86_H_

#include "update.h"

#define NUM_ELEM_LS86 4

typedef struct {

    void (*destroy)(void **dest);
    int ina[NUM_ELEM_LS86];
    vallist *ina_rootptr[NUM_ELEM_LS86];
    int inb[NUM_ELEM_LS86];
    vallist *inb_rootptr[NUM_ELEM_LS86];
    int oldvalue[NUM_ELEM_LS86];
    int value[NUM_ELEM_LS86];
    ehandler *out_event_handler_root[NUM_ELEM_LS86];
} ls86;

ls86 *ls86_create();

void ls86_destroy (ls86 **dest);

void ls86_connect_y1(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls86_connect_y2(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls86_connect_y3(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls86_connect_y4(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls86_in_a1(ls86 *dest, int *valptr, int timestamp);
void ls86_in_b1(ls86 *dest, int *valptr, int timestamp);
void ls86_in_a2(ls86 *dest, int *valptr, int timestamp);
void ls86_in_b2(ls86 *dest, int *valptr, int timestamp);
void ls86_in_a3(ls86 *dest, int *valptr, int timestamp);
void ls86_in_b3(ls86 *dest, int *valptr, int timestamp);
void ls86_in_a4(ls86 *dest, int *valptr, int timestamp);
void ls86_in_b4(ls86 *dest, int *valptr, int timestamp);

#endif /* LS86_H_ */
