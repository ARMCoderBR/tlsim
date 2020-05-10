/*
 * ls32.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls32.h"

// QUAD OR GATE

////////////////////////////////////////////////////////////////////////////////
static int ls32_update(ls32 *a, int index){

    a->value[index] = a->ina[index] | a->inb[index];

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void ls32_up(ls32 *a, timevalue_t timestamp, int index){

    if (ls32_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.valueptr = &a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls32_update_pin_ina(ls32 *a, bitvalue_t *valptr, timevalue_t timestamp, int index){

    bitvalue_t val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls32_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls32_update_pin_inb(ls32 *a, bitvalue_t *valptr, timevalue_t timestamp, int index){

    bitvalue_t val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls32_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls32 *ls32_create(){

    ls32 *b = malloc(sizeof(ls32));

    if (b == NULL)
        return NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS32; i++){

        b->ina[i] = b->inb[i] = 2;
        b->value[i] = b->oldvalue[i] = 1;
        b->ina_rootptr[i] = NULL;
        b->inb_rootptr[i] = NULL;
        b->out_event_handler_root[i] = NULL;
    }

    b->destroy = (void*)ls32_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls32_destroy (ls32 **dest){

    if (dest == NULL) return;
    ls32 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_ELEM_LS32; i++){

        vallist_destroy(&b->ina_rootptr[i]);
        vallist_destroy(&b->inb_rootptr[i]);
        ehandler_destroy(&b->out_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls32_connect_y1(ls32 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_connect_y2(ls32 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_connect_y3(ls32 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_connect_y4(ls32 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_a1(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_b1(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_a2(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_b2(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_a3(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_ina(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_b3(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_inb(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_a4(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_ina(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls32_in_b4(ls32 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls32_update_pin_inb(dest, valptr, timestamp, 3);
}
