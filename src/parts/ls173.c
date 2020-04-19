/*
 * ls173.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "ls173.h"

// 4-BIT D-TYPE REGISTERS WITH 3-STATE OUTPUTS

////////////////////////////////////////////////////////////////////////////////
static void ls173_update (ls173 *a, int timestamp){

    int i;

    if (a->in_clr){

        for (i = 0; i < NUM_ELEM_LS173; i++)
            a->outq[i] = a->latd[i] = 0;

        goto ls173_end;
    }

    ////////////////////////////////
    if (a->clk_o != a->clk){

        a->clk_o = a->clk;

        if ((a->clk) && (!a->in_g1) && (!a->in_g2))
            for (i = 0; i < NUM_ELEM_LS173; i++)
                a->latd[i] = a->inpd[i];
    }

    ////////////////////////////////
    if (a->in_m | a->in_n){

        for (i = 0; i < NUM_ELEM_LS173; i++)
            a->outq[i] = 2; //HiZ
    }
    else{
        for (i = 0; i < NUM_ELEM_LS173; i++)
            a->outq[i] = a->latd[i];
    }

    ////////////////////////////////
    event e;

ls173_end:
    e.timestamp = timestamp+1;

    for (i = 0; i < NUM_ELEM_LS173; i++){

        if (a->outq_o[i] != a->outq[i]){

            a->outq_o[i] = a->outq[i];
            e.event_handler_root = a->outq_event_handler_root[i];
            e.valueptr = &a->outq[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls173_update_d(ls173 *a, int *valptr, int timestamp, int index){

    int val = update_val_multi(&a->inpd_rootptr[index], valptr);

    if (val > 1) val = 1;

    a->inpd[index] = val;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls173 *ls173_create(){

    ls173 *b = malloc(sizeof(ls173));

    if (b == NULL)
        return NULL;

    int i;

    b->clk = b->clk_o = 1;
    b->in_m = b->in_n = 1;
    b->in_g1 = b->in_g2 = 1;

    b->in_clr_rootptr = b->in_m_rootptr = b->in_n_rootptr = b->in_g1_rootptr = b->in_g2_rootptr = b->clk_rootptr = NULL;

    for (i = 0; i < NUM_ELEM_LS173; i++){

        b->inpd_rootptr[i] = NULL;
        b->outq[i] = b->outq_o[i] = 2;
        b->latd[i] = 0;
        b->outq_event_handler_root[i] = NULL;
    }

    return b;
}

////////////////////////////////////////////////////////////////////////////////
static void ls173_connect_q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_1q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_2q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_3q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_4q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_1d(ls173 *dest, int *valptr, int timestamp){

    ls173_update_d(dest, valptr, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_2d(ls173 *dest, int *valptr, int timestamp){

    ls173_update_d(dest, valptr, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_3d(ls173 *dest, int *valptr, int timestamp){

    ls173_update_d(dest, valptr, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_4d(ls173 *dest, int *valptr, int timestamp){

    ls173_update_d(dest, valptr, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_clk(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->clk_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->clk == val) return;

    dest->clk = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_clr(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_clr_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_clr == val) return;

    dest->in_clr = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_m(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_m_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_m == val) return;

    dest->in_m = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_n(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_n_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_n == val) return;

    dest->in_n = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_g1(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_g1_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_g1 == val) return;

    dest->in_g1 = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_in_g2(ls173 *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->in_g2_rootptr, valptr);

    if (val > 1) val = 1;

    if (dest->in_g2 == val) return;

    dest->in_g2 = val;
    ls173_update(dest, timestamp);
}

