/*
 * ls161.h
 *
 *  Created on: 29 de abr de 2020
 *      Author: milton
 */

#ifndef LS161_H_
#define LS161_H_

#include "update.h"

typedef struct {

    //Entradas
    int da, db, dc, dd, load, enp, ent, clk, clear;
	vallist *da_rootptr, *db_rootptr, *dc_rootptr, *dd_rootptr, *load_rootptr, *enp_rootptr, *ent_rootptr, *clk_rootptr, *clear_rootptr;
	//Estado
    int clk_o;
    //Saídas
    int qa,qb,qc,qd,ripclk;
    int qa_o,qb_o,qc_o,qd_o,ripclk_o;
    ehandler *qa_event_handler_root;
    ehandler *qb_event_handler_root;
    ehandler *qc_event_handler_root;
    ehandler *qd_event_handler_root;
    ehandler *ripclk_event_handler_root;
} ls161;

ls161 *ls161_create();

void ls161_destroy(ls161 **dest);

void ls161_connect_qa(ls161 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls161_connect_qb(ls161 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls161_connect_qc(ls161 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls161_connect_qd(ls161 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));
void ls161_connect_ripclk(ls161 *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ls161_in_da(ls161 *dest, int *valptr, int timestamp);
void ls161_in_db(ls161 *dest, int *valptr, int timestamp);
void ls161_in_dc(ls161 *dest, int *valptr, int timestamp);
void ls161_in_dd(ls161 *dest, int *valptr, int timestamp);
void ls161_in_load(ls161 *dest, int *valptr, int timestamp);
void ls161_in_enp(ls161 *dest, int *valptr, int timestamp);
void ls161_in_ent(ls161 *dest, int *valptr, int timestamp);
void ls161_in_clk(ls161 *dest, int *valptr, int timestamp);
void ls161_in_clear(ls161 *dest, int *valptr, int timestamp);

#endif /* LS161_H_ */
