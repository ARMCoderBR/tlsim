/*
 * ls189.h
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#ifndef LS189_H_
#define LS189_H_

#include "update.h"

#define NUM_BITS_LS189 4
#define NUM_WORDS_LS189 16

typedef struct {

    void (*destroy)(void **dest);
    int inpd[NUM_BITS_LS189];
	vallist *inpd_rootptr[NUM_BITS_LS189];

    int in_addr[4];
    vallist *in_addr_rootptr[4];
    int in_cs, in_we;
    vallist *in_cs_rootptr, *in_we_rootptr;
    int current_addr;

	int latd[NUM_WORDS_LS189][NUM_BITS_LS189];
    int outq[NUM_BITS_LS189];
    int outq_o[NUM_BITS_LS189];
    ehandler *outq_event_handler_root[NUM_BITS_LS189];
    char name[32];
} ls189;

ls189 *ls189_create(char *name);

void ls189_destroy (ls189 **dest);

void ls189_connect_1q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls189_connect_2q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls189_connect_3q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls189_connect_4q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls189_in_1d(ls189 *dest, int *valptr, int timestamp);
void ls189_in_2d(ls189 *dest, int *valptr, int timestamp);
void ls189_in_3d(ls189 *dest, int *valptr, int timestamp);
void ls189_in_4d(ls189 *dest, int *valptr, int timestamp);

void ls189_in_a0(ls189 *dest, int *valptr, int timestamp);
void ls189_in_a1(ls189 *dest, int *valptr, int timestamp);
void ls189_in_a2(ls189 *dest, int *valptr, int timestamp);
void ls189_in_a3(ls189 *dest, int *valptr, int timestamp);

void ls189_in_cs(ls189 *dest, int *valptr, int timestamp);
void ls189_in_we(ls189 *dest, int *valptr, int timestamp);

#endif /* LS189_H_ */
