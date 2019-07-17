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
    b->oldvalue = 2;
    b->out_event_handler_root = NULL;
    return b;
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_connect_out(bitswitch *s, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(s->out_event_handler_root, obj, event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_setval(bitswitch *s, int val){

    s->value = val;

    if (s->oldvalue != s->value){

        s->oldvalue = s->value;
        event e;
        e.event_handler_root = s->out_event_handler_root;
        e.value = s->value;
        e.timestamp = 0;
        event_insert(&e);
    }
}
