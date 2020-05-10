/*
 * ls76.h
 *
 *  Created on: 03 de mai de 2020
 *      Author: milton
 */

#ifndef LS76_H_
#define LS76_H_

#include "update.h"

#define NUM_BITS_LS76 2

typedef struct {

    part_destroy_function_t destroy;
    //Entradas
    bitvalue_t pre[NUM_BITS_LS76], clr[NUM_BITS_LS76], clk[NUM_BITS_LS76], j[NUM_BITS_LS76], k[NUM_BITS_LS76];
	vallist *pre_rootptr[NUM_BITS_LS76], *clr_rootptr[NUM_BITS_LS76], *clk_rootptr[NUM_BITS_LS76], *j_rootptr[NUM_BITS_LS76], *k_rootptr[NUM_BITS_LS76];
	//Estado
	bitvalue_t clk_o[NUM_BITS_LS76];
    //Saídas
	bitvalue_t q[NUM_BITS_LS76],qn[NUM_BITS_LS76];
    ehandler *q_event_handler_root[NUM_BITS_LS76];
    ehandler *qn_event_handler_root[NUM_BITS_LS76];
} ls76;

ls76 *ls76_create();

void ls76_destroy(ls76 **dest);

void ls76_connect_1q(ls76 *source, void *dest, event_function_t dest_event_handler);

void ls76_connect_1qn(ls76 *source, void *dest, event_function_t dest_event_handler);

void ls76_in_1j(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_1k(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_1pre(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_1clr(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_1clk(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_connect_2q(ls76 *source, void *dest, event_function_t dest_event_handler);

void ls76_connect_2qn(ls76 *source, void *dest, event_function_t dest_event_handler);

void ls76_in_2j(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_2k(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_2pre(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_2clr(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls76_in_2clk(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS76_H_ */
