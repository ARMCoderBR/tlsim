/*
 * ls283.c
 *
 *  Created on: 18 de abr de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls283.h"

// 4-BIT BINARY ADDER WITH FAST CARRY

////////////////////////////////////////////////////////////////////////////////
static void ls283_up(ls283 *a, int timestamp){

    int suma, sumb;
    int mask = 1;
    int i = 0;

    suma = sumb = 0;

    for (i = 0; i < LS283_NBITS; i++){

        if (a->ina[i])
            suma |= mask;

        if (a->inb[i])
            sumb |= mask;

        mask <<= 1;
    }

    suma += sumb;

    if (a->cin)
        suma++;

    mask = 1;

    for (i = 0; i < LS283_NBITS; i++){

        if (suma & mask)
            a->y[i] = 1;
        else
            a->y[i] = 0;

        if (a->y[i] != a->oldy[i]){

            a->oldy[i] = a->y[i];
            event e;
            e.event_handler_root = a->y_event_handler_root[i];
            e.valueptr = &a->y[i];
            e.timestamp = timestamp+1;
            event_insert(&e);
        }

        mask <<= 1;
    }

    if (suma & mask)
        a->cout = 1;
    else
        a->cout = 0;

    if (a->cout != a->oldcout){

        a->oldcout = a->cout;
        event e;
        e.event_handler_root = a->cout_event_handler_root;
        e.valueptr = &a->cout;
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls283_update_pin_ina(ls283 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls283_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void ls283_update_pin_inb(ls283 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls283_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void ls283_update_pin_cin(ls283 *a, int *valptr, int timestamp){

    int val = update_val_multi(&a->cin_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->cin) return;

    a->cin = val;
    ls283_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls283 *ls283_create(){

    ls283 *b = malloc(sizeof(ls283));

    if (b == NULL)
        return NULL;

    int i;

    for (i = 0; i < LS283_NBITS; i++){

        b->y[i] = 0;
        b->ina_rootptr[i] = b->inb_rootptr[i] = NULL;
        b->oldy[i] = 2;
        b->y_event_handler_root[i] = NULL;
    }

    b->cout = 0;
    b->cin_rootptr = NULL;
    b->oldcout = 2;
    b->cout_event_handler_root = NULL;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls283_connect_y1(ls283 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->y[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_connect_y2(ls283 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->y[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_connect_y3(ls283 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->y[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_connect_y4(ls283 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->y[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_connect_cout(ls283 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->cout_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->cout,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_a1(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_a2(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_a3(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_ina(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_a4(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_ina(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_b1(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_b2(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_b3(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_inb(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_b4(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_inb(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls283_in_cin(ls283 *dest, int *valptr, int timestamp){

    ls283_update_pin_cin(dest, valptr, timestamp);
}
