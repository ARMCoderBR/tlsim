/*
 * ls173.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS173_H_
#define LS173_H_

#include "update.h"

#define NUM_ELEM_LS173 4

typedef struct {

    int inpd[NUM_ELEM_LS173];
	vallist *inpd_rootptr[NUM_ELEM_LS173];
    int in_clr;
    int in_m;
    int in_n;
    int in_g1;
    int in_g2;
    int clk;
	vallist *in_clr_rootptr, *in_m_rootptr, *in_n_rootptr, *in_g1_rootptr, *in_g2_rootptr, *clk_rootptr;
	int clk_o;
    int latd[NUM_ELEM_LS173];
    int outq_o[NUM_ELEM_LS173];
    int outq[NUM_ELEM_LS173];
    ehandler *outq_event_handler_root[NUM_ELEM_LS173];
} ls173;

ls173 *ls173_create();

void ls173_connect_1q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls173_connect_2q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls173_connect_3q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls173_connect_4q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls173_in_1d(ls173 *dest, int *valptr, int timestamp);
void ls173_in_2d(ls173 *dest, int *valptr, int timestamp);
void ls173_in_3d(ls173 *dest, int *valptr, int timestamp);
void ls173_in_4d(ls173 *dest, int *valptr, int timestamp);

void ls173_in_clk(ls173 *dest, int *valptr, int timestamp);
void ls173_in_clr(ls173 *dest, int *valptr, int timestamp);

void ls173_in_m(ls173 *dest, int *valptr, int timestamp);
void ls173_in_n(ls173 *dest, int *valptr, int timestamp);
void ls173_in_g1(ls173 *dest, int *valptr, int timestamp);
void ls173_in_g2(ls173 *dest, int *valptr, int timestamp);

#endif /* LS173_H_ */
