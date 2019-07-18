/*
 * and.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include "bitand.h"

////////////////////////////////////////////////////////////////////////////////
bitand *bitand_create(){

    bitand *b = malloc(sizeof(bitand));
    b->oldvalue = 2;
    b->out_event_handler_root = NULL;
    return b;
}

////////////////////////////////////////////////////////////////////////////////
void bitand_connect_out(bitand *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->out_event_handler_root, obj, event_handler);
    event_handler(obj,a->value,0);
}

////////////////////////////////////////////////////////////////////////////////
int bitand_update(bitand *a){

    a->value = a->ina & a->inb;

    if (a->oldvalue != a->value){

        a->oldvalue = a->value;
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void bitand_up(bitand *a, int val, int timestamp){

    if (bitand_update(a)){

        event e;
        e.event_handler_root = a->out_event_handler_root;
        e.value = a->value;
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void bitand_update_ina(bitand *a, int val, int timestamp){

    if (val == a->ina) return;

    a->ina = val;
    bitand_up(a, val, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void bitand_update_inb(bitand *a, int val, int timestamp){

    if (val == a->inb) return;

    a->inb = val;
    bitand_up(a, val, timestamp);
}
