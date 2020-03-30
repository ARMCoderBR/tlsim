/*
 * ls32.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS32_H_
#define LS32_H_

#include "update.h"

typedef struct {

    int ina[4];
    int inb[4];
    int oldvalue[4];
    int value[4];
    ehandler *out_event_handler_root[4];
} ls32;

ls32 *ls32_create();

void ls32_connect_y1(ls32 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls32_connect_y2(ls32 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls32_connect_y3(ls32 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls32_connect_y4(ls32 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls32_in_a1(ls32 *dest, int val, int timestamp);
void ls32_in_b1(ls32 *dest, int val, int timestamp);
void ls32_in_a2(ls32 *dest, int val, int timestamp);
void ls32_in_b2(ls32 *dest, int val, int timestamp);
void ls32_in_a3(ls32 *dest, int val, int timestamp);
void ls32_in_b3(ls32 *dest, int val, int timestamp);
void ls32_in_a4(ls32 *dest, int val, int timestamp);
void ls32_in_b4(ls32 *dest, int val, int timestamp);

#endif /* LS32_H_ */
