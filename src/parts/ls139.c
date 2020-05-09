/*
 * ls139.c
 *
 *  Created on: 03 de mai de 2020
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "ls139.h"

// DUAL 2-LINE TO 4-LINE DECODERS/DEMULTIPLEXERS

////////////////////////////////////////////////////////////////////////////////
static void ls139_up(ls139 *a, int timestamp, int index){

    a->y0[index] = a->y1[index] = a->y2[index] = a->y3[index] = 1;

    if (a->ing[index])
        goto end139;

    int code = (a->inb[index]?2:0) +
            (a->ina[index]?1:0);

    switch(code){

    case 0:
        a->y0[index] = 0;
        break;
    case 1:
        a->y1[index] = 0;
        break;
    case 2:
        a->y2[index] = 0;
        break;
    case 3:
        a->y3[index] = 0;
        break;
    }

end139:

    if (a->oy0[index] != a->y0[index]){

        a->oy0[index] = a->y0[index];
        event e;
        e.event_handler_root = a->y0_event_handler_root[index];
        e.valueptr = &a->y0[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }

    if (a->oy1[index] != a->y1[index]){

        a->oy1[index] = a->y1[index];
        event e;
        e.event_handler_root = a->y1_event_handler_root[index];
        e.valueptr = &a->y1[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }

    if (a->oy2[index] != a->y2[index]){

        a->oy2[index] = a->y2[index];
        event e;
        e.event_handler_root = a->y2_event_handler_root[index];
        e.valueptr = &a->y2[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }

    if (a->oy3[index] != a->y3[index]){

        a->oy3[index] = a->y3[index];
        event e;
        e.event_handler_root = a->y3_event_handler_root[index];
        e.valueptr = &a->y3[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls139_update_pin_ina(ls139 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls139_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls139_update_pin_inb(ls139 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls139_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls139_update_pin_ing(ls139 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->ing_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ing[index]) return;

    a->ing[index] = val;
    ls139_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls139 *ls139_create(){

    ls139 *b = malloc(sizeof(ls139));

    if (b == NULL)
        return NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS139; i++){

        b->ina[i] = b->inb[i] = b->ing[i] = 2;
        b->ina_rootptr[i] = b->inb_rootptr[i] = b->ing_rootptr[i] = NULL;
        b->y0[i] = b->y1[i] = b->y2[i] = b->y3[i] = 1;
        b->oy0[i] = b->oy1[i] = b->oy2[i] = b->oy3[i] = 1;
        b->y0_event_handler_root[i] = b->y1_event_handler_root[i] = b->y2_event_handler_root[i] = b->y3_event_handler_root[i] = NULL;
    }

    b->destroy = (void*)ls139_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls139_destroy (ls139 **dest){

    if (dest == NULL) return;
    ls139 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_ELEM_LS139; i++){

        vallist_destroy(&b->ina_rootptr[i]);
        vallist_destroy(&b->inb_rootptr[i]);
        vallist_destroy(&b->ing_rootptr[i]);
        ehandler_destroy(&b->y0_event_handler_root[i]);
        ehandler_destroy(&b->y1_event_handler_root[i]);
        ehandler_destroy(&b->y2_event_handler_root[i]);
        ehandler_destroy(&b->y3_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_1y0(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y0_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->y0[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_1y1(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y1_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->y1[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_1y2(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y2_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->y2[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_1y3(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y3_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->y3[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_2y0(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y0_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->y0[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_2y1(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y1_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->y1[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_2y2(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y2_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->y2[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_connect_2y3(ls139 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->y3_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->y3[1],0);
}



////////////////////////////////////////////////////////////////////////////////
void ls139_in_1a(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_in_1b(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_in_1g(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_ing(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_in_2a(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_in_2b(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls139_in_2g(ls139 *dest, int *valptr, int timestamp){

    ls139_update_pin_ing(dest, valptr, timestamp, 1);
}

