/*
 * ls189.c
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>

#include "update.h"
#include "ls189.h"

// 64-BIT RANDOM ACCESS MEMORY WITH 3-STATE OUTPUTS

////////////////////////////////////////////////////////////////////////////////
static void ls189_update (ls189 *a, int timestamp){

    int i;

    if (a->in_cs){  // Not selected

        for (i = 0; i < NUM_BITS_LS189; i++)
            a->outq[i] = 2;

        goto ls189_end;
    }

    ////////////////////////////////
    if (!a->in_we){ // Write

        for (i = 0; i < NUM_BITS_LS189; i++){

            a->latd[a->current_addr][i] = a->inpd[i];
            a->outq[i] = 2;
        }
        goto ls189_end;
    }

    // Read
    for (i = 0; i < NUM_BITS_LS189; i++){

        a->outq[i] = a->latd[a->current_addr][i] ? 0 : 1; //Inverted value!
    }

    ////////////////////////////////
    event e;

ls189_end:
    e.timestamp = timestamp+1;

    for (i = 0; i < NUM_BITS_LS189; i++){

        if (a->outq_o[i] != a->outq[i]){

            a->outq_o[i] = a->outq[i];
            e.event_handler_root = a->outq_event_handler_root[i];
            e.valueptr = &a->outq[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls189_update_d(ls189 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inpd_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (a->inpd[index] == val) return;

    logger("ls189_update_d%d [%s] *valptr:%d val:%d TS:%d",index,a->name,*valptr,val,timestamp);

    a->inpd[index] = val;

    ls189_update(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
static void ls189_update_addr(ls189 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->in_addr_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (a->in_addr[index] == val) return;

    logger("ls189_update_addr%d [%s] *valptr:%d val:%d TS:%d",index,a->name,*valptr,val,timestamp);

    a->in_addr[index] = val;

    a->current_addr = (a->in_addr[3] << 3) | (a->in_addr[2] << 2) | (a->in_addr[1] << 1) | (a->in_addr[0]);

    ls189_update(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls189 *ls189_create(char *name){

    ls189 *b = malloc(sizeof(ls189));

    if (b == NULL)
        return NULL;

    int i;

    b->in_addr[3] = b->in_addr[2] = b->in_addr[1] = b->in_addr[0] = 1;
    b->current_addr = 0x0f;
    b->in_cs = b->in_we = 1;

    b->in_addr_rootptr[3] = b->in_addr_rootptr[2] = b->in_addr_rootptr[1] = b->in_addr_rootptr[0] = NULL;
    b->in_cs_rootptr = b->in_we_rootptr = NULL;

    for (i = 0; i < NUM_BITS_LS189; i++){

        b->inpd[i] = 1;
        b->inpd_rootptr[i] = NULL;
        b->outq[i] = b->outq_o[i] = 2;
        b->outq_event_handler_root[i] = NULL;
    }

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
static void ls189_connect_q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_connect_1q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls189_connect_q(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_connect_2q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls189_connect_q(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_connect_3q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls189_connect_q(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_connect_4q(ls189 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls189_connect_q(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_1d(ls189 *dest, int *valptr, int timestamp){

    ls189_update_d(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_2d(ls189 *dest, int *valptr, int timestamp){

    ls189_update_d(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_3d(ls189 *dest, int *valptr, int timestamp){

    ls189_update_d(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_4d(ls189 *dest, int *valptr, int timestamp){

    ls189_update_d(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_a0(ls189 *dest, int *valptr, int timestamp){

    ls189_update_addr(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_a1(ls189 *dest, int *valptr, int timestamp){

    ls189_update_addr(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_a2(ls189 *dest, int *valptr, int timestamp){

    ls189_update_addr(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_a3(ls189 *dest, int *valptr, int timestamp){

    ls189_update_addr(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_we(ls189 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_we_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_we == val) return;

    logger("ls189_in_we [%s] *valptr:%d val:%d TS:%d",dest->name,*valptr,val,timestamp);

    dest->in_we = val;
    ls189_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls189_in_cs(ls189 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_cs_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_cs == val) return;

    logger("ls189_in_cs [%s] *valptr:%d val:%d TS:%d",dest->name,*valptr,val,timestamp);

    dest->in_cs = val;
    ls189_update(dest, timestamp);
}
