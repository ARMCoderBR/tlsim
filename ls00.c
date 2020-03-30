/*
 * ls00.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls00.h"

// QUAD NAND GATE

////////////////////////////////////////////////////////////////////////////////
static int ls00_update(ls00 *a, int index){

    a->value[index] = 1 ^ (a->ina[index] & a->inb[index]);

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void ls00_up(ls00 *a, int timestamp, int index){

    if (ls00_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.value = a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls00_update_pin_ina(ls00 *a, int val, int timestamp, int index){

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls00_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls00_update_pin_inb(ls00 *a, int val, int timestamp, int index){

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls00_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls00 *ls00_create(){

    ls00 *b = malloc(sizeof(ls00));

    if (b == NULL)
        return NULL;

    b->oldvalue[0] = b->oldvalue[1] = b->oldvalue[2] = b->oldvalue[3] = 2;
    b->out_event_handler_root[0] = b->out_event_handler_root[1] =
            b->out_event_handler_root[2] = b->out_event_handler_root[3] = NULL;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls00_connect_y1(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_connect_y2(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_connect_y3(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_connect_y4(ls00 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_a1(ls00 *dest, int val, int timestamp){

    ls00_update_pin_ina(dest, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_b1(ls00 *dest, int val, int timestamp){

    ls00_update_pin_inb(dest, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_a2(ls00 *dest, int val, int timestamp){

    ls00_update_pin_ina(dest, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_b2(ls00 *dest, int val, int timestamp){

    ls00_update_pin_inb(dest, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_a3(ls00 *dest, int val, int timestamp){

    ls00_update_pin_ina(dest, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_b3(ls00 *dest, int val, int timestamp){

    ls00_update_pin_inb(dest, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_a4(ls00 *dest, int val, int timestamp){

    ls00_update_pin_ina(dest, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls00_update_b4(ls00 *dest, int val, int timestamp){

    ls00_update_pin_inb(dest, val, timestamp, 3);
}
