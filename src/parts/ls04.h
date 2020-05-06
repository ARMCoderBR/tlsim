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

    int in[NUM_ELEM_LS04];
    vallist *in_rootptr[NUM_ELEM_LS04];
    int oldvalue[NUM_ELEM_LS04];
    int value[NUM_ELEM_LS04];
    ehandler *out_event_handler_root[NUM_ELEM_LS04];
} ls04;

ls04 *ls04_create();

void ls04_destroy (ls04 **dest);

void ls04_connect_y1(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls04_connect_y2(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls04_connect_y3(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls04_connect_y4(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls04_connect_y5(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls04_connect_y6(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls04_in_a1(ls04 *dest, int *valptr, int timestamp);
void ls04_in_a2(ls04 *dest, int *valptr, int timestamp);
void ls04_in_a3(ls04 *dest, int *valptr, int timestamp);
void ls04_in_a4(ls04 *dest, int *valptr, int timestamp);
void ls04_in_a5(ls04 *dest, int *valptr, int timestamp);
void ls04_in_a6(ls04 *dest, int *valptr, int timestamp);

#endif /* LS04_H_ */
