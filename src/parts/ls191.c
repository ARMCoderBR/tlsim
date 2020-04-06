/*
 * ls191.c
 *
 *  Created on: 18 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "ls191.h"

// UP/DOWN BINARY COUNTER

////////////////////////////////////////////////////////////////////////////////
static int ls191_update(ls191 *a){

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

            if (!a->downup){
                //UP
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
            }else{
                //DOWN
                if (a->qa == 2)
                    a->qa = 0;
                else{
                    a->qb++;
                    if (a->qb == 2)
                        a->qb = 0;
                    else{
                        a->qc++;
                        if (a->qc == 2)
                            a->qc = 0;
                        else{
                            a->qd++;
                            if (a->qd == 2)
                                a->qd = 0;
                        }
                    }
                }
            }
        }
    }

    a->clk_o = a->clk;

    if (!a->downup){ //0=UP  1=DOWN

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
static void ls191_up(ls191 *a, int timestamp){

    event e;
    e.timestamp = timestamp+1;

    int mask = ls191_update(a);
    if (mask & 0x01){

        e.event_handler_root = a->qa_event_handler_root;
        e.valueptr = &a->qa;
        event_insert(&e);
    }

    if (mask & 0x02){

        e.event_handler_root = a->qb_event_handler_root;
        e.valueptr = &a->qb;
        event_insert(&e);
    }

    if (mask & 0x04){

        e.event_handler_root = a->qc_event_handler_root;
        e.valueptr = &a->qc;
        event_insert(&e);
    }

    if (mask & 0x08){

        e.event_handler_root = a->qd_event_handler_root;
        e.valueptr = &a->qd;
        event_insert(&e);
    }

    if (mask & 0x10){

        e.event_handler_root = a->maxmin_event_handler_root;
        e.valueptr = &a->maxmin;
        event_insert(&e);
    }

    if (mask & 0x20){

        e.event_handler_root = a->ripclk_event_handler_root;
        e.valueptr = &a->ripclk;
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls191 *ls191_create(){

    ls191 *b = malloc(sizeof(ls191));

    if (b == NULL){
        return NULL;
    }

	b->da_rootptr = b->db_rootptr = b->dc_rootptr = b->dd_rootptr = b->load_rootptr = b->enable_rootptr = b->downup_rootptr = b->clk_rootptr = NULL;
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
void ls191_connect_qa(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->qa_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->qa,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qb(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->qb_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->qb,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qc(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->qc_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->qc,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_qd(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->qd_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->qd,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_maxmin(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->maxmin_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->maxmin,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_connect_ripclk(ls191 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->ripclk_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->ripclk,0);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_da(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->da_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->da) return;
    dest->da = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_db(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->db_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->db) return;
    dest->db = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_dc(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->dc_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->dc) return;
    dest->dc = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_dd(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->dd_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->dd) return;
    dest->dd = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_load(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->load_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->load) return;
    dest->load = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_enable(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->enable_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->enable) return;
    dest->enable = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_updown(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->downup_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->downup) return;
    dest->downup = val;
    ls191_up(dest,timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls191_in_clk(ls191 *dest, int *valptr, int timestamp){

    int val = update_val_multi(dest->clk_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == dest->clk) return;
    dest->clk = val;
    ls191_up(dest,timestamp);
}
