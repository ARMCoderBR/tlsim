/*
 * ls173.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <malloc.h>
#include "ls173.h"

// 4-BIT D-TYPE REGISTERS WITH 3-STATE OUTPUTS

////////////////////////////////////////////////////////////////////////////////
static void ls173_update (ls173 *a, int timestamp){

    int i;

    ////////////////////////////////
    if (a->clk_o != a->clk){

        a->clk_o = a->clk;

        if (a->clk)
            for (i = 0; i < 8; i++)
                a->latd[i] = a->inpd[i];
    }

    ////////////////////////////////
    if (a->in_ocon){

        for (i = 0; i < 4; i++)
            a->outq[i] = 2; //HiZ
    }
    else{
        for (i = 0; i < 4; i++)
            a->outq[i] = a->latd[i];
    }

    ////////////////////////////////
    event e;
    e.timestamp = timestamp+1;

    for (i = 0; i < 4; i++){

        if (a->outq_o[i] != a->outq[i]){

            e.event_handler_root = a->outq_event_handler_root[i];
            e.value = a->outq[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls173_update_d(ls173 *a, int val, int timestamp, int index){

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
    b->in_ocon = 1;

    for (i = 0; i < 4; i++){

        b->outq[i] = b->outq_o[i] = 2;
        b->latd[i] = 0;
        b->outq_event_handler_root[i] = NULL;
    }

    return b;
}

////////////////////////////////////////////////////////////////////////////////
static void ls173_connect_q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,source->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_1q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_2q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_3q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_4q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_5q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_6q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_7q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_connect_8q(ls173 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls173_connect_q(source, dest, dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_1d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_2d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_3d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_4d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_5d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_6d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_7d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_8d(ls173 *dest, int val, int timestamp){

    ls173_update_d(dest, val, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_clk(ls173 *dest, int val, int timestamp){

    if (val > 1) val = 1;

    if (dest->clk == val) return;

    dest->clk = val;
    ls173_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls173_update_ocon(ls173 *dest, int val, int timestamp){

    if (val > 1) val = 1;

    if (dest->in_ocon == val) return;

    dest->in_ocon = val;
    ls173_update(dest, timestamp);
}
