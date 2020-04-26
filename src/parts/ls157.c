/*
 * ls157.c
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "ls157.h"

// QUADRUPLE 2-LINE TO 1-LINE DATA SELECTORS/MULTIPLEXERS

////////////////////////////////////////////////////////////////////////////////
static void ls157_up(ls157 *a, int timestamp){

    int i;

    if (a->in_g){

        for (i = 0; i < NUM_ELEM_LS157; i++)
            a->value[i] = 2;
    }
    else
    if (a->in_sel){

        for (i = 0; i < NUM_ELEM_LS157; i++)
            a->value[i] = a->inb[i];
    }
    else{

        for (i = 0; i < NUM_ELEM_LS157; i++)
            a->value[i] = a->ina[i];
    }

    for (i = 0; i < NUM_ELEM_LS157; i++){

        if (a->value[i] != a->oldvalue[i]){

            a->oldvalue[i] = a->value[i];

            event e;
            e.event_handler_root = a->out_event_handler_root[i];
            e.valueptr = &a->value[i];
            e.timestamp = timestamp+1;
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls157_update_pin_ina(ls157 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls157_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void ls157_update_pin_inb(ls157 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls157_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls157 *ls157_create(){

    ls157 *b = malloc(sizeof(ls157));

    if (b == NULL)
        return NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS157; i++){

        b->ina[i] = b->inb[i] = 1;
        b->value[i] = b->oldvalue[i] = 2;
        b->ina_rootptr[i] = NULL;
        b->inb_rootptr[i] = NULL;
        b->out_event_handler_root[i] = NULL;
    }

    b->in_sel = b->in_g = 1;
    b->in_sel_rootptr = b->in_g_rootptr = NULL;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls157_connect_y1(ls157 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_connect_y2(ls157 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_connect_y3(ls157 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_connect_y4(ls157 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_a1(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_b1(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_a2(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_b2(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_a3(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_ina(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_b3(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_inb(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_a4(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_ina(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_b4(ls157 *dest, int *valptr, int timestamp){

    ls157_update_pin_inb(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_sel(ls157 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_sel_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->in_sel) return;

    dest->in_sel = val;
    ls157_up(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls157_in_g(ls157 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_g_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->in_g) return;

    dest->in_g = val;
    ls157_up(dest, timestamp);
}
