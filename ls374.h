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

    int in1d,in2d,in3d,in4d,in5d,in6d,in7d,in8d;
    int in_ocon;
    int clk,clk_o;
    int out1q_o,out2q_o,out3q_o,out4q_o,out5q_o,out6q_o,out7q_o,out8q_o;
    int out1q,out2q,out3q,out4q,out5q,out6q,out7q,out8q;
    ehandler *out1q_event_handler_root,*out2q_event_handler_root,*out3q_event_handler_root,*out4q_event_handler_root,
             *out5q_event_handler_root,*out6q_event_handler_root,*out7q_event_handler_root,*out8q_event_handler_root;
} ls374;

ls374 *ls374_create();

void ls191_connect_1d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_2d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_3d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_4d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_5d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_6d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_7d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_8d(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_clk(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_ocon(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

void ls191_update_1q(ls374 *a, int val, int timestamp);
void ls191_update_2q(ls374 *a, int val, int timestamp);
void ls191_update_3q(ls374 *a, int val, int timestamp);
void ls191_update_4q(ls374 *a, int val, int timestamp);
void ls191_update_5q(ls374 *a, int val, int timestamp);
void ls191_update_6q(ls374 *a, int val, int timestamp);
void ls191_update_7q(ls374 *a, int val, int timestamp);
void ls191_update_8q(ls374 *a, int val, int timestamp);

#endif /* LS374_H_ */
