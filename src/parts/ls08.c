////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>

#include "update.h"
#include "ls08.h"

// QUAD AND GATE

////////////////////////////////////////////////////////////////////////////////
static int ls08_update(ls08 *a, int index){

    a->value[index] = a->ina[index] & a->inb[index];

    if (a->oldvalue[index] != a->value[index]){

        a->oldvalue[index] = a->value[index];
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
static void ls08_up(ls08 *a, int timestamp, int index){

    if (ls08_update(a,index)){

        event e;
        e.event_handler_root = a->out_event_handler_root[index];
        e.valueptr = &a->value[index];
        e.timestamp = timestamp+1;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls08_update_pin_ina(ls08 *a, bitvalue_t *valptr, timevalue_t timestamp, int index){

    bitvalue_t val = update_val_multi(&a->ina_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->ina[index]) return;

    a->ina[index] = val;
    ls08_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
static void ls08_update_pin_inb(ls08 *a, bitvalue_t *valptr, timevalue_t timestamp, int index){

    bitvalue_t val = update_val_multi(&a->inb_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == a->inb[index]) return;

    a->inb[index] = val;
    ls08_up(a, timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls08 *ls08_create(){

    ls08 *b = malloc(sizeof(ls08));

    if (b == NULL)
        return NULL;

    int i;
    for (i = 0; i < NUM_ELEM_LS08; i++){

        b->ina[i] = b->inb[i] = 2;
        b->value[i] = b->oldvalue[i] = 1;
        b->ina_rootptr[i] = NULL;
        b->inb_rootptr[i] = NULL;
        b->out_event_handler_root[i] = NULL;
    }

    b->destroy = (void*)ls08_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls08_destroy (ls08 **dest){

    if (dest == NULL) return;
    ls08 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_ELEM_LS08; i++){

        vallist_destroy(&b->ina_rootptr[i]);
        vallist_destroy(&b->inb_rootptr[i]);
        ehandler_destroy(&b->out_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y1(ls08 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[0], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[0],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y2(ls08 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[1], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[1],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y3(ls08 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[2], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[2],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_connect_y4(ls08 *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[3], dest, dest_event_handler);
    dest_event_handler(dest,&source->value[3],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_a1(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_ina(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_b1(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_inb(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_a2(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_ina(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_b2(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_inb(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_a3(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_ina(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_b3(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_inb(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_a4(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_ina(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls08_in_b4(ls08 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls08_update_pin_inb(dest, valptr, timestamp, 3);
}
