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

    ls00 *ls00;
    ls04 *ls04;
    bitswitch *reset_sw;
    indicator *led[NSIGNALS_CTRU];
    vallist *in_rootptr[NSIGNALS_CTRU];
    ehandler *out_event_handler_root[NSIGNALS_CTRU];
    int val[NSIGNALS_CTRU];
    int o_val[NSIGNALS_CTRU];
    char name[30];

} ctrunit;

ctrunit *ctrunit_create(char *name);

board_object *ctrunit_board_create(ctrunit *reg, int key, char *name); // Requer NCURSES

#endif /* REG_8BIT_H_ */
