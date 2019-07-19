/*
 * ls191.c
 *
 *  Created on: 18 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include "ls191.h"

////////////////////////////////////////////////////////////////////////////////
ls191 *ls191_create(){

    ls191 *b = malloc(sizeof(ls191));
    b->qa_o = b->qb_o = b->qc_o = b->qd_o = b->maxmin_o = b->ripclk_o = 2;
    b->qa_event_handler_root =
    b->qb_event_handler_root =
    b->qc_event_handler_root =
    b->qd_event_handler_root =
    b->maxmin_event_handler_root =
    b->ripclk_event_handler_root = NULL;
    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qa(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->qa_event_handler_root, obj, event_handler);
    event_handler(obj,a->qa,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qb(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->qb_event_handler_root, obj, event_handler);
    event_handler(obj,a->qb,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qc(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->qc_event_handler_root, obj, event_handler);
    event_handler(obj,a->qc,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qd(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->qd_event_handler_root, obj, event_handler);
    event_handler(obj,a->qd,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_maxmin(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->maxmin_event_handler_root, obj, event_handler);
    event_handler(obj,a->maxmin,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_ripclk(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->ripclk_event_handler_root, obj, event_handler);
    event_handler(obj,a->ripclk,0);
}

////////////////////////////////////////////////////////////////////////////////
int ls191_update(ls191 *a){

    if (!a->load){

        a->qa = a->da;
        a->qb = a->db;
        a->qc = a->dc;
        a->qd = a->dd;
    }
    else{

        if (!a->enable)
        if ((!a->clk_o) && (a->clk)){

            a->qa++;
            if (a->qa == 2) {
                a->qa = 0;
                a->qb++;
                if (a->qb == 2){
                    a->qb = 0;
                    a->qc++;
                    if (a->qc == 2){
                        a->qc = 0;
                        a->qd++;
                        if (a->qd == 2)
                            a->qd = 0;
                    }
                }
            }
        }
    }

    a->clk_o = a->clk;

    if (a->downup){ //0=UP  1=DOWN

        a->maxmin = (a->qa & a->qb & a->qc & a->qd);
    }else{

        a->maxmin = 1 ^ (a->qa | a->qb | a->qc | a->qd);
    }

    a->ripclk = 1;
    if ((a->maxmin) && (!a->clk))
        a->ripclk = 0;

    int mask = 0;
    if (a->qa_o != a->qa){
        a->qa_o = a->qa; mask |= 0x01;
    }
    if (a->qb_o != a->qb){
        a->qb_o = a->qb; mask |= 0x02;
    }
    if (a->qc_o != a->qc){
        a->qc_o = a->qc; mask |= 0x04;
    }
    if (a->qd_o != a->qd){
        a->qd_o = a->qd; mask |= 0x08;
    }
    if (a->maxmin_o != a->maxmin){
        a->maxmin_o = a->maxmin; mask |= 0x10;
    }
    if (a->ripclk_o != a->ripclk){
        a->ripclk_o = a->ripclk; mask |= 0x20;
    }

    return mask;
}

////////////////////////////////////////////////////////////////////////////////
void ls191_up(ls191 *a, int timestamp){

    event e;
    e.timestamp = timestamp+1;

    int mask = ls191_update(a);
    if (mask & 0x01){

        e.event_handler_root = a->qa_event_handler_root;
        e.value = a->qa;
        event_insert(&e);
    }

    if (mask & 0x02){

        e.event_handler_root = a->qb_event_handler_root;
        e.value = a->qb;
        event_insert(&e);
    }

    if (mask & 0x04){

        e.event_handler_root = a->qc_event_handler_root;
        e.value = a->qc;
        event_insert(&e);
    }

    if (mask & 0x08){

        e.event_handler_root = a->qd_event_handler_root;
        e.value = a->qd;
        event_insert(&e);
    }

    if (mask & 0x10){

        e.event_handler_root = a->maxmin_event_handler_root;
        e.value = a->maxmin;
        event_insert(&e);
    }

    if (mask & 0x20){

        e.event_handler_root = a->ripclk_event_handler_root;
        e.value = a->ripclk;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_da(ls191 *a, int val, int timestamp){

    if (val == a->da) return;
    a->da = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_db(ls191 *a, int val, int timestamp){

    if (val == a->db) return;
    a->db = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_dc(ls191 *a, int val, int timestamp){

    if (val == a->dc) return;
    a->dc = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_dd(ls191 *a, int val, int timestamp){

    if (val == a->dd) return;
    a->dd = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_load(ls191 *a, int val, int timestamp){

    if (val == a->load) return;
    a->load = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_enable(ls191 *a, int val, int timestamp){

    if (val == a->enable) return;
    a->enable = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_downup(ls191 *a, int val, int timestamp){

    if (val == a->downup) return;
    a->downup = val;
    ls191_up(a,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_update_clk(ls191 *a, int val, int timestamp){

    if (val == a->clk) return;
    a->clk = val;
    ls191_up(a,timestamp);
}
