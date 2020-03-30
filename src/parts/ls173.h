/*
 * ls173.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS173_H_
#define LS173_H_

#include "update.h"

typedef struct {

    int inpd[4];
    int latd[4];
    int in_ocon;
    int clk,clk_o;
    int outq_o[4];
    int outq[4];
    ehandler *outq_event_handler_root[4];
} ls173;

ls173 *ls173_create();

void ls173_connect_1q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls173_connect_2q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls173_connect_3q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls173_connect_4q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls173_in_1d(ls173 *dest, int val, int timestamp);
void ls173_in_2d(ls173 *dest, int val, int timestamp);
void ls173_in_3d(ls173 *dest, int val, int timestamp);
void ls173_in_4d(ls173 *dest, int val, int timestamp);

void ls173_in_clk(ls173 *dest, int val, int timestamp);
void ls173_update_clr(ls173 *dest, int val, int timestamp);

void ls173_update_m(ls173 *dest, int val, int timestamp);
void ls173_update_n(ls173 *dest, int val, int timestamp);
void ls173_update_g1(ls173 *dest, int val, int timestamp);
void ls173_update_g2(ls173 *dest, int val, int timestamp);

#endif /* LS173_H_ */
