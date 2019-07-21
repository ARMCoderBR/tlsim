/*
 * ls08.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "ls08.h"

////////////////////////////////////////////////////////////////////////////////
ls08 *ls08_create(){

    ls08 *b = malloc(sizeof(ls08));
    b->oldvalue[0] = b->oldvalue[1] = b->oldvalue[2] = b->oldvalue[3] = 2;
    b->out_event_handler_root[0] = b->out_event_handler_root[1] =
            b->out_event_handler_root[2] = b->out_event_handler_root[3] = NULL;
    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y1(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->out_event_handler_root[0], obj, event_handler);
    event_handler(obj,a->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y2(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->out_event_handler_root[1], obj, event_handler);
    event_handler(obj,a->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y3(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->out_event_handler_root[2], obj, event_handler);
    event_handler(obj,a->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y4(ls08 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->out_event_handler_root[3], obj, event_handler);
    event_handler(obj,a->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
int ls08_update(ls08 *a, int index){

    a->value[index] = a->ina[index] & a->inb[index];

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ls08_up(ls08 *a, int timestamp, int index){

    if (ls08_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.value = a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_ina(ls08 *a, int val, int timestamp, int index){

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls08_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_inb(ls08 *a, int val, int timestamp, int index){

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls08_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_a1(ls08 *a, int val, int timestamp){

    ls08_update_ina(a, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_b1(ls08 *a, int val, int timestamp){

    ls08_update_inb(a, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_a2(ls08 *a, int val, int timestamp){

    ls08_update_ina(a, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_b2(ls08 *a, int val, int timestamp){

    ls08_update_inb(a, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_a3(ls08 *a, int val, int timestamp){

    ls08_update_ina(a, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_b3(ls08 *a, int val, int timestamp){

    ls08_update_inb(a, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_a4(ls08 *a, int val, int timestamp){

    ls08_update_ina(a, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_update_b4(ls08 *a, int val, int timestamp){

    ls08_update_inb(a, val, timestamp, 3);
}
