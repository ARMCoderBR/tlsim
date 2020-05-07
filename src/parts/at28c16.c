/*
 * at28c16.c
 *
 *  Created on: 02 de mai de 2020
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>

#include "update.h"
#include "at28c16.h"

// 16K (2Kx8) PARALLEL EEPROMS

// Notes: Does not simulate special 12V operations (Chip Clear / Read ID)
//        Does not take into account timing constraints (WE pulse width, setup/hold times, etc).

////////////////////////////////////////////////////////////////////////////////
static void at28c16_wr_cell(at28c16 *a){

    int i;
    int b = 0;
    int msk = 1;
    for (i = 0; i < NUM_BITS_28C16; i++){

        if (a->inpd[i])
            b |= msk;
        msk <<= 1;
        a->outq[i] = 2;
    }
    a->eep_byte[a->current_addr] = b;
}

////////////////////////////////////////////////////////////////////////////////
static void at28c16_update (at28c16 *a, int timestamp){

    int i;

    if (a->in_cs){  // Not selected

        for (i = 0; i < NUM_BITS_28C16; i++)
            a->outq[i] = 2;

        if (!a->in_cs_old){

            if (!a->in_we){ // End of Write, CS Controlled

                if (a->in_oe)
                    at28c16_wr_cell(a);
            }

            a->in_cs_old = a->in_cs;
        }

        goto at28c16_end;
    }

    a->in_cs_old = a->in_cs;

    ////////////////////////////////
    if (!a->in_we){

        for (i = 0; i < NUM_BITS_28C16; i++)
            a->outq[i] = 2;

        a->in_we_old = a->in_we;
        goto at28c16_end;
    }
    else
    if (!a->in_we_old){ // Write

        a->in_we_old = a->in_we;
        if (a->in_oe)
            at28c16_wr_cell(a);
        goto at28c16_end;
    }

    if (a->in_oe){  // OE Not selected

        for (i = 0; i < NUM_BITS_28C16; i++)
            a->outq[i] = 2;

        goto at28c16_end;
    }

    // Read
    int msk = 1;
    for (i = 0; i < NUM_BITS_28C16; i++){

        if (a->eep_byte[a->current_addr] & msk)
            a->outq[i] = 1;
        else
            a->outq[i] = 0;
        msk <<= 1;
    }

    ////////////////////////////////
    event e;

at28c16_end:
    e.timestamp = timestamp+1;

    for (i = 0; i < NUM_BITS_28C16; i++){

        if (a->outq_o[i] != a->outq[i]){

            a->outq_o[i] = a->outq[i];
            e.event_handler_root = a->outq_event_handler_root[i];
            e.valueptr = &a->outq[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void at28c16_update_d(at28c16 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inpd_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (a->inpd[index] == val) return;

    logger("at28c16_update_d%d [%s] *valptr:%d val:%d TS:%d",index,a->name,*valptr,val,timestamp);

    a->inpd[index] = val;

    at28c16_update(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void at28c16_update_addr(at28c16 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->in_addr_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (a->in_addr[index] == val) return;

    logger("at28c16_update_addr%d [%s] *valptr:%d val:%d TS:%d",index,a->name,*valptr,val,timestamp);

    a->in_addr[index] = val;

    a->current_addr = 0;
    int i;
    for (i = 10; i >= 0; i--){

        a->current_addr <<= 1;
        if (a->in_addr[i])
            a->current_addr++;
    }
    //a->current_addr = (a->in_addr[3] << 3) | (a->in_addr[2] << 2) | (a->in_addr[1] << 1) | (a->in_addr[0]);

    at28c16_update(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
at28c16 *at28c16_create(char *name, unsigned char *template){

    at28c16 *b = malloc(sizeof(at28c16));

    if (b == NULL)
        return NULL;

    int i;

    for (i = 0; i < 11; i++){

        b->in_addr[i] = 2;
        b->in_addr_rootptr[i] = NULL;
    }

    b->current_addr = 0x7f;
    b->in_cs = b->in_we = b->in_oe = b->in_we_old = b->in_cs_old = 2;
    b->in_cs_rootptr = b->in_we_rootptr = b->in_oe_rootptr = NULL;

    for (i = 0; i < NUM_BITS_28C16; i++){

        b->inpd[i] = 2;
        b->inpd_rootptr[i] = NULL;
        b->outq[i] = b->outq_o[i] = 2;
        b->outq_event_handler_root[i] = NULL;
    }

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

    if (!template)
        memset(b->eep_byte,0xff,NUM_WORDS_28C16);
    else
        memcpy(b->eep_byte,template,NUM_WORDS_28C16);

    b->destroy = (void*)at28c16_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_destroy(at28c16 **dest){

    if (dest == NULL) return;
    at28c16 *b = *dest;
    if (b == NULL) return;

    int i;

    for (i = 0; i < 11; i++){

        b->in_addr[i] = 2;
        vallist_destroy(&b->in_addr_rootptr[i]);
    }

    vallist_destroy(&b->in_cs_rootptr);
    vallist_destroy(&b->in_we_rootptr);
    vallist_destroy(&b->in_oe_rootptr);

    for (i = 0; i < NUM_BITS_28C16; i++){

    	vallist_destroy(&b->inpd_rootptr[i]);
        ehandler_destroy(&b->outq_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
static void at28c16_connect_q(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o0(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o1(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o2(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o3(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o4(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o5(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o6(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_connect_o7(at28c16 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    at28c16_connect_q(source, dest, dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i0(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i1(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i2(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i3(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i4(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i5(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i6(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_i7(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_d(dest, valptr, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a0(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a1(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a2(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a3(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a4(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a5(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a6(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a7(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a8(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 8);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a9(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 9);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_a10(at28c16 *dest, int *valptr, int timestamp){

    at28c16_update_addr(dest, valptr, timestamp, 10);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_we(at28c16 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_we_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_we == val) return;

    logger("at28c16_in_we [%s] *valptr:%d val:%d TS:%d",dest->name,*valptr,val,timestamp);

    dest->in_we = val;
    at28c16_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_oe(at28c16 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_oe_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_oe == val) return;

    logger("at28c16_in_oe [%s] *valptr:%d val:%d TS:%d",dest->name,*valptr,val,timestamp);

    dest->in_oe = val;
    at28c16_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void at28c16_in_cs(at28c16 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_cs_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_cs == val) return;

    logger("at28c16_in_cs [%s] *valptr:%d val:%d TS:%d",dest->name,*valptr,val,timestamp);

    dest->in_cs = val;
    at28c16_update(dest, timestamp);
}
