////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef CTRUNIT_H_
#define CTRUNIT_H_

// Based on Ben Eater's project (https://www.youtube.com/watch?v=tNwU7pK_3tk)

#include "ls00.h"
#include "ls04.h"
#include "indicator.h"
#include "bitswitch.h"
#include "board.h"
#include "ls161.h"
#include "ls138.h"
#include "ls173.h"
#include "at28c16.h"

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
#define FI  0

#define HHLT 0x80
#define HMI  0x40
#define HRI  0x20
#define HRO  0x10
#define HIO  0x08
#define HII  0x04
#define HAI  0x02
#define HAO  0x01

#define LSO  0x80
#define LSU  0x40
#define LBI  0x20
#define LOI  0x10
#define LCE  0x08
#define LCO  0x04
#define LJ   0x02
#define LFI  0x01

typedef struct {

    part_destroy_function_t destroy;
    ls00 *ls00;
    ls04 *ls04_1, *ls04_2;
    bitswitch *reset_sw;
    indicator *led[NSIGNALS_CTRU];
    vallist *in_rootptr[NSIGNALS_CTRU];
    ehandler *out_event_handler_root[NSIGNALS_CTRU];
    bitvalue_t val[NSIGNALS_CTRU];
    bitvalue_t o_val[NSIGNALS_CTRU];
    char name[30];
    /////////////////////////
    ls161 *ls161;
    ls138 *ls138;
    indicator *ct[3];
    indicator *t[6];
    indicator *ledclk;
    at28c16 *eep_hi, *eep_lo;
    ls173 *ls173;
    indicator *ledz, *ledc;
    event_context_t *ec;
} ctrunit;

ctrunit *ctrunit_create(event_context_t *ec, char *name);

void ctrunit_destroy (ctrunit **dest);

board_object *ctrunit_board_create(ctrunit *reg, int key, char *name); // Requer NCURSES

board_object *ctrunit_board_flags_create(ctrunit *reg, int key, char *name); // Requer NCURSES

void ctrunit_in_hlt(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_hlt(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_mi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_mi(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ri(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ri(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ro(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ro(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_io(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_io(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ii(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ii(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ai(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ai(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ao(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ao(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_so(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_so(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_su(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_su(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_bi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_bi(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_oi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_oi(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_ce(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_ce(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_co(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_co(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_j(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_connect_out_j(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_clk(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ctrunit_in_clkn(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_fi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_connect_out_reset(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_connect_out_nreset(ctrunit *source, void *dest, event_function_t dest_event_handler);

void ctrunit_in_instr0(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_instr1(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_instr2(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_instr3(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_zero(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ctrunit_in_carry(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* CTRUNIT_H_ */
