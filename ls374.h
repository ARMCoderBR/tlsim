/*
 * ls374.h
 *
 *  Created on: 21 de jul de 2019
 *      Author: milton
 */

#ifndef LS374_H_
#define LS374_H_

#include "update.h"

typedef struct {

    int in1d[8];
    int in_ocon;
    int clk,clk_o;
    int outq_o[8];
    int outq[8];
    ehandler *outq_event_handler_root[8];
} ls374;

ls374 *ls374_create();

void ls374_connect_1q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_2q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_3q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_4q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_5q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_6q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_7q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls374_connect_8q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

void ls374_update_1q(ls374 *a, int val, int timestamp);
void ls374_update_2q(ls374 *a, int val, int timestamp);
void ls374_update_3q(ls374 *a, int val, int timestamp);
void ls374_update_4q(ls374 *a, int val, int timestamp);
void ls374_update_5q(ls374 *a, int val, int timestamp);
void ls374_update_6q(ls374 *a, int val, int timestamp);
void ls374_update_7q(ls374 *a, int val, int timestamp);
void ls374_update_8q(ls374 *a, int val, int timestamp);
void ls374_update_clk(ls374 *a, int val, int timestamp);
void ls374_update_ocon(ls374 *a, int val, int timestamp);

#endif /* LS374_H_ */
