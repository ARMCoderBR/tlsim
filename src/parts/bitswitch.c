/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "bitswitch.h"
#include "update.h"

////////////////////////////////////////////////////////////////////////////////
bitswitch *bitswitch_create(){

    bitswitch *b = malloc(sizeof(bitswitch));

    if (b == NULL)
        return NULL;

    b->oldvalue = 2;
    b->out_event_handler_root = NULL;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_connect_out(bitswitch *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->value,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_setval(bitswitch *s, int val, int timestamp){

    s->value = val;

    if (s->oldvalue != s->value){

        s->oldvalue = s->value;
        event e;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = timestamp;
        event_insert(&e);
    }
}
