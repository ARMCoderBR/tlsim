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

    int inpd[8];
    int latd[8];
    int in_ocon;
    int clk,clk_o;
    int outq_o[8];
    int outq[8];
    ehandler *outq_event_handler_root[8];
} ls374;

ls374 *ls374_create();

void ls374_connect_1q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_2q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_3q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_4q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_5q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_6q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_7q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls374_connect_8q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls374_in_1d(ls374 *dest, int val, int timestamp);
void ls374_in_2d(ls374 *dest, int val, int timestamp);
void ls374_in_3d(ls374 *dest, int val, int timestamp);
void ls374_in_4d(ls374 *dest, int val, int timestamp);
void ls374_in_5d(ls374 *dest, int val, int timestamp);
void ls374_in_6d(ls374 *dest, int val, int timestamp);
void ls374_in_7d(ls374 *dest, int val, int timestamp);
void ls374_in_8d(ls374 *dest, int val, int timestamp);
void ls374_in_clk(ls374 *dest, int val, int timestamp);
void ls374_in_ocon(ls374 *dest, int val, int timestamp);

#endif /* LS374_H_ */
