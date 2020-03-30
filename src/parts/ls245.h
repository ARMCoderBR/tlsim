/*
 * ls245.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef LS245_H_
#define LS245_H_

#include "update.h"

typedef struct {

    int inpa[8];
    int inpb[8];
    int outa_o[8];
    int outa[8];
    int outb_o[8];
    int outb[8];
    int in_dir;
    int in_oe;
    ehandler *outq_event_handler_root[8+8];
} ls245;

ls245 *ls245_create();

void ls245_connect_a1(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a2(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a3(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a4(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a5(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a6(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a7(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_a8(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls245_connect_b1(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b2(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b3(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b4(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b5(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b6(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b7(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));
void ls245_connect_b8(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp));

void ls245_in_a1(ls245 *dest, int val, int timestamp);
void ls245_in_a2(ls245 *dest, int val, int timestamp);
void ls245_in_a3(ls245 *dest, int val, int timestamp);
void ls245_in_a4(ls245 *dest, int val, int timestamp);
void ls245_in_a5(ls245 *dest, int val, int timestamp);
void ls245_in_a6(ls245 *dest, int val, int timestamp);
void ls245_in_a7(ls245 *dest, int val, int timestamp);
void ls245_in_a8(ls245 *dest, int val, int timestamp);

void ls245_in_b1(ls245 *dest, int val, int timestamp);
void ls245_in_b2(ls245 *dest, int val, int timestamp);
void ls245_in_b3(ls245 *dest, int val, int timestamp);
void ls245_in_b4(ls245 *dest, int val, int timestamp);
void ls245_in_b5(ls245 *dest, int val, int timestamp);
void ls245_in_b6(ls245 *dest, int val, int timestamp);
void ls245_in_b7(ls245 *dest, int val, int timestamp);
void ls245_in_b8(ls245 *dest, int val, int timestamp);

void ls245_in_dir(ls245 *dest, int val, int timestamp);
void ls245_in_oe(ls245 *dest, int val, int timestamp);

#endif /* LS374_H_ */
