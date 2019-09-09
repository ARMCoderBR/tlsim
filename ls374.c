/*
 * ls374.c
 *
 *  Created on: 21 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include "ls374.h"

////////////////////////////////////////////////////////////////////////////////
static void ls374_update (ls374 *a, int timestamp){

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

        for (i = 0; i < 8; i++)
            a->outq[i] = 2; //HiZ
    }
    else{
        for (i = 0; i < 8; i++)
            a->outq[i] = a->latd[i];
    }

    ////////////////////////////////
    event e;
    e.timestamp = timestamp+1;

    for (i = 0; i < 8; i++){

        if (a->outq_o[i] != a->outq[i]){

            e.event_handler_root = a->outq_event_handler_root[i];
            e.value = a->outq[i];
            event_insert(&e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
static void ls374_update_d(ls374 *a, int val, int timestamp, int index){

    a->inpd[index] = val;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls374 *ls374_create(){

    ls374 *b = malloc(sizeof(ls374));

    if (b == NULL)
        return NULL;

    int i;

    for (i = 0; i < 8; i++){

        b->clk = b->clk_o = 1;
        b->outq[i] = b->outq_o[i] = 2;
        b->latd[i] = 0;
        b->outq_event_handler_root[i] = NULL;
        b->in_ocon = 1;
    }

    return b;
}

////////////////////////////////////////////////////////////////////////////////
static void ls374_connect_q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp), int index){

    new_ehandler(&source->outq_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,source->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_1q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_2q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_3q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_4q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_5q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_6q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_7q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_8q(ls374 *source, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    ls374_connect_q(source, dest, dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_1d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_2d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_3d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_4d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_5d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_6d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_7d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_8d(ls374 *dest, int val, int timestamp){

    ls374_update_d(dest, val, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_clk(ls374 *dest, int val, int timestamp){

    if (dest->clk == val) return;

    dest->clk = val;
    ls374_update(dest, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_ocon(ls374 *dest, int val, int timestamp){

    if (dest->in_ocon == val) return;

    dest->in_ocon = val;
    ls374_update(dest, timestamp);
}
