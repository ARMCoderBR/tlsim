/*
 * ls86.c
 *
 *  Created on: 18 de abr de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls86.h"

// QUAD XOR GATE

////////////////////////////////////////////////////////////////////////////////
static int ls86_update(ls86 *a, int index){

    a->value[index] = a->ina[index] ^ a->inb[index];

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void ls86_up(ls86 *a, int timestamp, int index){

    if (ls86_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.valueptr = &a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls86_update_pin_ina(ls86 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls86_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls86_update_pin_inb(ls86 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls86_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls86 *ls86_create(){

    ls86 *b = malloc(sizeof(ls86));

    if (b == NULL)
        return NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS86; i++){

        b->ina[i] = b->inb[i] = 2;
        b->value[i] = b->oldvalue[i] = 0;
        b->ina_rootptr[i] = NULL;
        b->inb_rootptr[i] = NULL;
        b->out_event_handler_root[i] = NULL;
    }

    b->destroy = (void*)ls86_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls86_destroy (ls86 **dest){

    if (dest == NULL) return;
    ls86 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_ELEM_LS86; i++){

        vallist_destroy(&b->ina_rootptr[i]);
        vallist_destroy(&b->inb_rootptr[i]);
        ehandler_destroy(&b->out_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls86_connect_y1(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_connect_y2(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_connect_y3(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_connect_y4(ls86 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_a1(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_b1(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_a2(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_b2(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_a3(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_ina(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_b3(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_inb(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_a4(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_ina(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls86_in_b4(ls86 *dest, int *valptr, int timestamp){

    ls86_update_pin_inb(dest, valptr, timestamp, 3);
}
