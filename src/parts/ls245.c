/*
 * ls245.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "ls245.h"

// OCTAL BUS TRANSCEIVERS WITH 3-STATE OUTPUTS

////////////////////////////////////////////////////////////////////////////////
static void ls245_update (ls245 *a, int timestamp){

    int i;

    ////////////////////////////////
    if (a->in_oe){

        for (i = 0; i < NUM_ELEM_LS245; i++)
            a->outa[i] = a->outb[i] = 2; //HiZ
        goto end_ls245;
    }

    ////////////////////////////////
    if (a->in_dir){

        for (i = 0; i < NUM_ELEM_LS245; i++)
            a->outb[i] = a->inpa[i]?1:0;
    }
    else{

        for (i = 0; i < NUM_ELEM_LS245; i++)
            a->outa[i] = a->inpb[i]?1:0;
    }

    ////////////////////////////////
    event e;

end_ls245:
    e.timestamp = timestamp+1;

    for (i = 0; i < NUM_ELEM_LS245; i++){

        if (a->outa_o[i] != a->outa[i]){

            a->outa_o[i] = a->outa[i];
            e.event_handler_root = a->outq_event_handler_root[i];
            e.valueptr = &a->outa[i];
            event_insert(&e);
        }

        if (a->outb_o[i] != a->outb[i]){

            a->outb_o[i] = a->outb[i];
            e.event_handler_root = a->outq_event_handler_root[NUM_ELEM_LS245+i];
            e.valueptr = &a->outb[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls245_update_a(ls245 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inpa_rootptr[index], valptr);

    if (val > 1) val = 1;

    a->inpa[index] = val;
    ls245_update (a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void ls245_update_b(ls245 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inpb_rootptr[index], valptr);

    if (val > 1) val = 1;

    a->inpb[index] = val;
    ls245_update (a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls245 *ls245_create(){

    ls245 *b = malloc(sizeof(ls245));

    if (b == NULL)
        return NULL;

    b->in_dir = b->in_oe = 2;
    b->in_dir_rootptr = b->in_oe_rootptr = NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS245; i++){

        b->inpa[i] = b->inpb[i] = 2;
        b->inpa_rootptr[i] = NULL;
        b->inpb_rootptr[i] = NULL;
        b->outa[i] = b->outa_o[i] = 2;
        b->outb[i] = b->outb_o[i] = 2;
        b->outq_event_handler_root[i] = NULL;
        b->outq_event_handler_root[NUM_ELEM_LS245+i] = NULL;
    }

    b->destroy = (void*)ls245_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls245_destroy (ls245 **dest){

    if (dest == NULL) return;
    ls245 *b = *dest;
    if (b == NULL) return;

    vallist_destroy(&b->in_dir_rootptr);
    vallist_destroy(&b->in_oe_rootptr);

    int i;
    for (i = 0; i < NUM_ELEM_LS245; i++){

        vallist_destroy(&b->inpa_rootptr[i]);
        vallist_destroy(&b->inpb_rootptr[i]);
        ehandler_destroy(&b->outq_event_handler_root[i]);
        ehandler_destroy(&b->outq_event_handler_root[NUM_ELEM_LS245+i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
static void ls245_connect_a(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->outa[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a1(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a2(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a3(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a4(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a5(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a6(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a7(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_a8(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_a(source, dest, dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
static void ls245_connect_b(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[NUM_ELEM_LS245+index], dest, dest_event_handler);
    dest_event_handler(dest,&source->outb[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b1(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b2(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b3(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b4(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b5(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b6(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b7(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_connect_b8(ls245 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls245_connect_b(source, dest, dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a1(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a2(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a3(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a4(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a5(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a6(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a7(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_a8(ls245 *dest, int *valptr, int timestamp){

    ls245_update_a(dest, valptr, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b1(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b2(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b3(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b4(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b5(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b6(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b7(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_b8(ls245 *dest, int *valptr, int timestamp){

    ls245_update_b(dest, valptr, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_dir(ls245 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_dir_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_dir == val) return;

    dest->in_dir = val;
    ls245_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls245_in_oe(ls245 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_oe_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_oe == val) return;

    dest->in_oe = val;
    ls245_update(dest, timestamp);
}
