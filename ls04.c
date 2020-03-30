/*
 * ls04.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls04.h"

// HEX INVERTER

////////////////////////////////////////////////////////////////////////////////
static int ls04_update(ls04 *a, int index){

    a->value[index] = a->in[index] ^ 1;

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void ls04_up(ls04 *a, int timestamp, int index){

    if (ls04_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.value = a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls04_update_pin_in(ls04 *a, int val, int timestamp, int index){

    if (val > 1) val = 1;

    if (val == a->in[index]) return;

    a->in[index] = val;
    ls04_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls04 *ls04_create(){

    ls04 *b = malloc(sizeof(ls04));

    if (b == NULL)
        return NULL;

    b->oldvalue[0] = b->oldvalue[1] = b->oldvalue[2] = b->oldvalue[3] = b->oldvalue[4] = b->oldvalue[5] = 2;
    b->out_event_handler_root[0] = b->out_event_handler_root[1] =
            b->out_event_handler_root[2] = b->out_event_handler_root[3] =
                    b->out_event_handler_root[4] = b->out_event_handler_root[5] = NULL;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y1(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y2(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y3(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y4(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y5(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[4], dest, dest_event_handler);
    dest_event_handler(dest,source->value[4],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y6(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->out_event_handler_root[5], dest, dest_event_handler);
    dest_event_handler(dest,source->value[5],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a1(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a2(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a3(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a4(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a5(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_update_a6(ls04 *dest, int val, int timestamp){

    ls04_update_pin_in(dest, val, timestamp, 5);
}
