////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS191_H_
#define LS191_H_

#include "update.h"

typedef struct {

    part_destroy_function_t destroy;
    //Entradas
    bitvalue_t da, db, dc, dd, load, enable, downup, clk;
	vallist *da_rootptr, *db_rootptr, *dc_rootptr, *dd_rootptr, *load_rootptr, *enable_rootptr, *downup_rootptr, *clk_rootptr;
	//Estado
	bitvalue_t clk_o;
    //Saídas
	bitvalue_t qa,qb,qc,qd,maxmin,ripclk;
	bitvalue_t qa_o,qb_o,qc_o,qd_o,maxmin_o,ripclk_o;
    ehandler *qa_event_handler_root;
    ehandler *qb_event_handler_root;
    ehandler *qc_event_handler_root;
    ehandler *qd_event_handler_root;
    ehandler *maxmin_event_handler_root;
    ehandler *ripclk_event_handler_root;
} ls191;

ls191 *ls191_create();

void ls191_destroy (ls191 **dest);

void ls191_connect_qa(ls191 *source, void *dest, event_function_t dest_event_handler);
void ls191_connect_qb(ls191 *source, void *dest, event_function_t dest_event_handler);
void ls191_connect_qc(ls191 *source, void *dest, event_function_t dest_event_handler);
void ls191_connect_qd(ls191 *source, void *dest, event_function_t dest_event_handler);
void ls191_connect_maxmin(ls191 *source, void *dest, event_function_t dest_event_handler);
void ls191_connect_ripclk(ls191 *source, void *dest, event_function_t dest_event_handler);

void ls191_in_da(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_db(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_dc(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_dd(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_load(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_enable(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_updown(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls191_in_clk(ls191 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS191_H_ */
