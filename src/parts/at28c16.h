/*
 * at28c16.h
 *
 *  Created on: 02 de mai de 2020
 *      Author: milton
 */

#ifndef AT28C16_H_
#define AT28C16_H_

#include "update.h"

#define NUM_BITS_28C16 8
#define NUM_WORDS_28C16 2048

typedef struct {

    int inpd[NUM_BITS_28C16];
	vallist *inpd_rootptr[NUM_BITS_28C16];

    int in_addr[11];
    vallist *in_addr_rootptr[11];
    int in_cs, in_we, in_oe;
    vallist *in_cs_rootptr, *in_we_rootptr, *in_oe_rootptr;
    int current_addr;

	unsigned char eep_byte[NUM_WORDS_28C16];
    int outq[NUM_BITS_28C16];
    int outq_o[NUM_BITS_28C16];
    ehandler *outq_event_handler_root[NUM_BITS_28C16];
    char name[32];
} at28c16;

at28c16 *at28c16_create(char *name, unsigned char *template);

void at28c16_connect_o0(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o1(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o2(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o3(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o4(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o5(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o6(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void at28c16_connect_o7(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void at28c16_in_i0(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i1(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i2(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i3(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i4(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i5(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i6(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_i7(at28c16 *dest, int *valptr, int timestamp);

void at28c16_in_a0(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a1(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a2(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a3(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a4(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a5(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a6(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a7(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a8(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a9(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_a10(at28c16 *dest, int *valptr, int timestamp);

void at28c16_in_cs(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_we(at28c16 *dest, int *valptr, int timestamp);
void at28c16_in_oe(at28c16 *dest, int *valptr, int timestamp);

#endif /* at28c16_H_ */
