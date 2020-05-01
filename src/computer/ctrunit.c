/*
 * ctrunit.c
 *
 *  Created on: 01 de mai de 2020
 *      Author: milton
 */

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=AwUirxi9eBg)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ctrunit.h"

char labels[16][4] = {"", "J", "CO", "CE", "OI", "BI", "SU", "SO", "AO", "AI", "II", "IO", "RO", "RI", "MI", "HLT" };

////////////////////////////////////////////////////////////////////////////////
ctrunit *ctrunit_create(char *name){

    ctrunit *ctru = malloc (sizeof(ctrunit));
    if (!ctru) return ctru;

    ctru->ls00 = ls00_create();
    ctru->ls04 = ls04_create();
    ctru->reset_sw = bitswitch_create("Reset");

    int i;
    for (i = 0; i < NSIGNALS_CTRU; i++){

        ctru->led[i] = indicator_create(labels[i]);
        ctru->in_rootptr[i] = NULL; ctru->out_event_handler_root[i] = NULL;
    }

    return ctru;
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in(ctrunit *dest, int index, int *valptr, int timestamp){

    indicator_in_d0(dest->led[index], valptr, timestamp);

    int val = update_val_multi(&dest->in_rootptr[index], valptr);

    if (val > 1)
        val = 1;

    dest->val[index] = val;

    if (val == dest->o_val[index]) return;

    dest->o_val[index] = val;

    event e;
    e.event_handler_root = dest->out_event_handler_root[index];
    e.valueptr = &dest->val[index];
    e.timestamp = timestamp+1;
    event_insert(&e);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out(ctrunit *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->val[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_hlt(ctrunit *dest, int *valptr, int timestamp){

    ctrunit_in(dest, HLT, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_hlt_out(ctrunit *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    ctrunit_connect_out(source, HLT, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
board_object *ctrunit_board_create(ctrunit *reg, int key, char *name){

    board_object *board = board_create(62, 4, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 1; i < NSIGNALS_CTRU; i++){

        j = NSIGNALS_CTRU-i;
        sprintf(s,labels[i],i);
        board_add_led(board, reg->led[i],1+4*(j-1),1,s, LED_BLUE);
    }

    //board_add_led(board, reg->ledclk,35,1,"CLK", LED_BLUE);

    return board;
}
