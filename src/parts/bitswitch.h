/*
 * bitswitch.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef BITSWITCH_H_
#define BITSWITCH_H_

#include "update.h"

typedef struct{

    int oldvalue;
    int value;
    ehandler *out_event_handler_root;
} bitswitch;

bitswitch *bitswitch_create();

void bitswitch_connect_out(bitswitch *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void bitswitch_setval(bitswitch *s, int val);

#endif /* BITSWITCH_H_ */
