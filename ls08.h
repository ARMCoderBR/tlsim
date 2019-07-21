/*
 * ls08.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef LS08_H_
#define LS08_H_

#include "update.h"

typedef struct {

    int ina[4];
    int inb[4];
    int oldvalue[4];
    int value[4];
    ehandler *out_event_handler_root[4];
} ls08;

ls08 *ls08_create();

void ls08_connect_out1(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls08_connect_out2(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls08_connect_out3(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls08_connect_out4(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

void ls08_update_a1(ls08 *a, int val, int timestamp);
void ls08_update_b1(ls08 *a, int val, int timestamp);
void ls08_update_a2(ls08 *a, int val, int timestamp);
void ls08_update_b2(ls08 *a, int val, int timestamp);
void ls08_update_a3(ls08 *a, int val, int timestamp);
void ls08_update_b3(ls08 *a, int val, int timestamp);
void ls08_update_a4(ls08 *a, int val, int timestamp);
void ls08_update_b4(ls08 *a, int val, int timestamp);

#endif /* LS08_H_ */
