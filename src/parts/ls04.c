/*
 * ls04.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
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
        e.valueptr = &a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls04_update_pin_in(ls04 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->in_rootptr[index], valptr);

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

    int i;
    for (i = 0; i < NUM_ELEM_LS04; i++){

        b->oldvalue[i] = 2;
        b->in_rootptr[i] = NULL;
        b->out_event_handler_root[i] = NULL;
    }

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y1(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y2(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y3(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y4(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y5(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[4], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[4],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_connect_y6(ls04 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[5], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[5],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a1(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a2(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a3(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a4(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a5(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls04_in_a6(ls04 *dest, int *valptr, int timestamp){

    ls04_update_pin_in(dest, valptr, timestamp, 5);
}
