/*
 * ls00.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS00_H_
#define LS00_H_

#include "update.h"

typedef struct {

    int ina[4];
    int inb[4];
    int oldvalue[4];
    int value[4];
    ehandler *out_event_handler_root[4];
} ls00;

ls00 *ls00_create();

void ls00_connect_y1(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls00_connect_y2(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls00_connect_y3(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls00_connect_y4(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls00_in_a1(ls00 *dest, int val, int timestamp);
void ls00_in_b1(ls00 *dest, int val, int timestamp);
void ls00_in_a2(ls00 *dest, int val, int timestamp);
void ls00_in_b2(ls00 *dest, int val, int timestamp);
void ls00_in_a3(ls00 *dest, int val, int timestamp);
void ls00_in_b3(ls00 *dest, int val, int timestamp);
void ls00_in_a4(ls00 *dest, int val, int timestamp);
void ls00_in_b4(ls00 *dest, int val, int timestamp);

#endif /* LS00_H_ */
