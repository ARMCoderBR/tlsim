////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS173_H_
#define LS173_H_

#include "update.h"

#define NUM_ELEM_LS173 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t inpd[NUM_ELEM_LS173];
	vallist *inpd_rootptr[NUM_ELEM_LS173];
	bitvalue_t in_clr;
	bitvalue_t in_m;
	bitvalue_t in_n;
	bitvalue_t in_g1;
	bitvalue_t in_g2;
	bitvalue_t clk;
	vallist *in_clr_rootptr, *in_m_rootptr, *in_n_rootptr, *in_g1_rootptr, *in_g2_rootptr, *clk_rootptr;
	bitvalue_t clk_o;
	bitvalue_t latd[NUM_ELEM_LS173];
	bitvalue_t outq_o[NUM_ELEM_LS173];
	bitvalue_t outq[NUM_ELEM_LS173];
    ehandler *outq_event_handler_root[NUM_ELEM_LS173];
    char name[32];
    event_context_t *ec;
} ls173;

ls173 *ls173_create(event_context_t *ec, char *name);

void ls173_destroy(ls173 **dest);

void ls173_connect_1q(ls173 *source, void *dest, event_function_t dest_event_handler);
void ls173_connect_2q(ls173 *source, void *dest, event_function_t dest_event_handler);
void ls173_connect_3q(ls173 *source, void *dest, event_function_t dest_event_handler);
void ls173_connect_4q(ls173 *source, void *dest, event_function_t dest_event_handler);

void ls173_in_1d(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_2d(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_3d(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_4d(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls173_in_clk(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_clr(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls173_in_m(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_n(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_g1(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls173_in_g2(ls173 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS173_H_ */
