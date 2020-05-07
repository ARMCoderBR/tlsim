/*
 * ls139.h
 *
 *  Created on: 03 de mai de 2020
 *      Author: milton
 */

#ifndef LS139_H_
#define LS139_H_

#include "update.h"

#define NUM_ELEM_LS139 2

typedef struct {

    void (*destroy)(void **dest);
    int ina[NUM_ELEM_LS139];
    vallist *ina_rootptr[NUM_ELEM_LS139];
    int inb[NUM_ELEM_LS139];
    vallist *inb_rootptr[NUM_ELEM_LS139];
    int ing[NUM_ELEM_LS139];
    vallist *ing_rootptr[NUM_ELEM_LS139];
    int y0[NUM_ELEM_LS139];
    int y1[NUM_ELEM_LS139];
    int y2[NUM_ELEM_LS139];
    int y3[NUM_ELEM_LS139];
    int oy0[NUM_ELEM_LS139];
    int oy1[NUM_ELEM_LS139];
    int oy2[NUM_ELEM_LS139];
    int oy3[NUM_ELEM_LS139];
    ehandler *y0_event_handler_root[NUM_ELEM_LS139];
    ehandler *y1_event_handler_root[NUM_ELEM_LS139];
    ehandler *y2_event_handler_root[NUM_ELEM_LS139];
    ehandler *y3_event_handler_root[NUM_ELEM_LS139];
} ls139;

ls139 *ls139_create();

void ls139_destroy(ls139 **dest);

void ls139_connect_1y0(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_1y1(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_1y2(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_1y3(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_2y0(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_2y1(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_2y2(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls139_connect_2y3(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls139_in_1a(ls139 *dest, int *valptr, int timestamp);
void ls139_in_1b(ls139 *dest, int *valptr, int timestamp);
void ls139_in_1g(ls139 *dest, int *valptr, int timestamp);
void ls139_in_2a(ls139 *dest, int *valptr, int timestamp);
void ls139_in_2b(ls139 *dest, int *valptr, int timestamp);
void ls139_in_2g(ls139 *dest, int *valptr, int timestamp);

#endif /* LS139_H_ */
