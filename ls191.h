/*
 * ls191.h
 *
 *  Created on: 18 de jul de 2019
 *      Author: milton
 */

#ifndef LS191_H_
#define LS191_H_

#include "update.h"

typedef struct {

    //Entradas
    int da, db, dc, dd, load, enable, downup, clk;
    int clk_old;
    //Saídas
    int qa,qb,qc,qd,maxmin,ripclk;
    int qa_o,qb_o,qc_o,qd_o,maxmin_o,ripclk_o;
    ehandler *qa_event_handler_root;
    ehandler *qb_event_handler_root;
    ehandler *qc_event_handler_root;
    ehandler *qd_event_handler_root;
    ehandler *maxmin_event_handler_root;
    ehandler *ripclk_event_handler_root;
} ls191;

ls191 *ls191_create();

void ls191_connect_qa(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_qb(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_qc(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_qd(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_maxmin(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));
void ls191_connect_ripclk(ls191 *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

void ls191_update_da(ls191 *a, int val, int timestamp);
void ls191_update_db(ls191 *a, int val, int timestamp);
void ls191_update_dc(ls191 *a, int val, int timestamp);
void ls191_update_dd(ls191 *a, int val, int timestamp);
void ls191_update_load(ls191 *a, int val, int timestamp);
void ls191_update_enable(ls191 *a, int val, int timestamp);
void ls191_update_downup(ls191 *a, int val, int timestamp);
void ls191_update_clk(ls191 *a, int val, int timestamp);

#endif /* LS191_H_ */
