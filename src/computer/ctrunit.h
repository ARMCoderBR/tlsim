/*
 * ctrunit.h
 *
 *  Created on: 01 de mai de 2020
 *      Author: milton
 */

#ifndef CTRUNIT_H_
#define CTRUNIT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=tNwU7pK_3tk)

#include "ls00.h"
#include "ls04.h"
#include "indicator.h"
#include "bitswitch.h"
#include "board.h"
#include "ls161.h"
#include "ls138.h"
#include "at28c16.h"

#define DISABLE_CTRUNIT_OUTS 0

#define NSIGNALS_CTRU 16

#define HLT 15
#define MI  14
#define RI  13
#define RO  12
#define IO  11
#define II  10
#define AI  9
#define AO  8
#define SO  7
#define SU  6
#define BI  5
#define OI  4
#define CE  3
#define CO  2
#define J   1


typedef struct {

    void (*destroy)(void **dest);
    ls00 *ls00;
    ls04 *ls04_1, *ls04_2;
    bitswitch *reset_sw;
    indicator *led[NSIGNALS_CTRU];
    vallist *in_rootptr[NSIGNALS_CTRU];
    ehandler *out_event_handler_root[NSIGNALS_CTRU];
    int val[NSIGNALS_CTRU];
    int o_val[NSIGNALS_CTRU];
    char name[30];
    /////////////////////////
    ls161 *ls161;
    ls138 *ls138;
    indicator *ct[3];
    indicator *t[6];
    indicator *ledclk;
    at28c16 *eep_hi, *eep_lo;
} ctrunit;

ctrunit *ctrunit_create(char *name);

void ctrunit_destroy (ctrunit **dest);

board_object *ctrunit_board_create(ctrunit *reg, int key, char *name); // Requer NCURSES

void ctrunit_in_hlt(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_hlt(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_mi(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_mi(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ri(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ri(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ro(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ro(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_io(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_io(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ii(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ii(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ai(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ai(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ao(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ao(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_so(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_so(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_su(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_su(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_bi(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_bi(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_oi(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_oi(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_ce(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_ce(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_co(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_co(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_j(ctrunit *dest, int *valptr, int timestamp);
void ctrunit_connect_out_j(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_clk(ctrunit *dest, int *valptr, int timestamp);

void ctrunit_connect_out_reset(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_connect_out_nreset(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ctrunit_in_instr0(ctrunit *dest, int *valptr, int timestamp);

void ctrunit_in_instr1(ctrunit *dest, int *valptr, int timestamp);

void ctrunit_in_instr2(ctrunit *dest, int *valptr, int timestamp);

void ctrunit_in_instr3(ctrunit *dest, int *valptr, int timestamp);

#endif /* REG_8BIT_H_ */
