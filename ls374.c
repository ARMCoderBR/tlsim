/*
 * ls374.c
 *
 *  Created on: 21 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include "ls374.h"

////////////////////////////////////////////////////////////////////////////////
ls374 *ls374_create(){

    ls374 *b = malloc(sizeof(ls374));

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
static void ls374_connect_q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp), int index){

    new_ehandler(&a->outq_event_handler_root[index], obj, event_handler);
    event_handler(obj,a->outq[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_1q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_2q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_3q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_4q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_5q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_6q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_7q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_connect_8q(ls374 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    ls374_connect_q(a, obj, event_handler, 7);
}

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
void ls374_update_1d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_2d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_3d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_4d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_5d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_6d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_7d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_8d(ls374 *a, int val, int timestamp){

    ls374_update_d(a, val, timestamp, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_clk(ls374 *a, int val, int timestamp){

    if (a->clk == val) return;

    a->clk = val;
    ls374_update(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls374_update_ocon(ls374 *a, int val, int timestamp){

    if (a->in_ocon == val) return;

    a->in_ocon = val;
    ls374_update(a, timestamp);
}
