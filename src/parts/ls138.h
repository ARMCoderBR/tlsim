/*
 * ls138.h
 *
 *  Created on: 09 de mai de 2020
 *      Author: milton
 */

#ifndef LS138_H_
#define LS138_H_

#include "update.h"

#define NUM_BITS_LS138 8

typedef struct {

    void (*destroy)(void **dest);
    int ina;
    vallist *ina_rootptr;
    int inb;
    vallist *inb_rootptr;
    int inc;
    vallist *inc_rootptr;
    int ing2a;
    vallist *ing2a_rootptr;
    int ing2b;
    vallist *ing2b_rootptr;
    int ing1;
    vallist *ing1_rootptr;
    int y[NUM_BITS_LS138];
    int oy[NUM_BITS_LS138];
    ehandler *y_event_handler_root[NUM_BITS_LS138];
} ls138;

ls138 *ls138_create();

void ls138_destroy(ls138 **dest);

void ls138_connect_y0(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y1(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y2(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y3(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y4(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y5(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y6(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls138_connect_y7(ls138 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls138_ina(ls138 *dest, int *valptr, int timestamp);
void ls138_inb(ls138 *dest, int *valptr, int timestamp);
void ls138_inc(ls138 *dest, int *valptr, int timestamp);
void ls138_ing2a(ls138 *dest, int *valptr, int timestamp);
void ls138_ing2b(ls138 *dest, int *valptr, int timestamp);
void ls138_ing1(ls138 *dest, int *valptr, int timestamp);

#endif /* LS138_H_ */
