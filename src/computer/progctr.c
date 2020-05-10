////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

// Based on Ben Eater's project (https://www.youtube.com/watch?v=tNwU7pK_3tk)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "progctr.h"
#include "bitconst.h"

////////////////////////////////////////////////////////////////////////////////
progctr *progctr_create(char *name){

    progctr *reg = malloc (sizeof(progctr));
    if (!reg) return reg;

    char lshi[60];
    char lslo[60];

    lshi[0] = 0;
    if (name){

        strncpy(lshi,name,sizeof(lshi));
    }

    strncat(lshi,"-Word",sizeof(lshi)/2);

    reg->ls161 = ls161_create(lshi);
    reg->ls245_1  = ls245_create();
    reg->ledclk = indicator_create("Clk");

    int i;
    for (i = 0; i < 4; i++){

        strncpy(lshi,name,sizeof(lshi));
        sprintf(lslo,"-D%d",i);
        strncat(lshi,lslo,sizeof(lshi)/2);
        reg->led[i] = indicator_create(lshi);
    }

    ls161_connect_qa(reg->ls161, reg->led[0], (void*)&indicator_in_d0);
    ls161_connect_qb(reg->ls161, reg->led[1], (void*)&indicator_in_d0);
    ls161_connect_qc(reg->ls161, reg->led[2], (void*)&indicator_in_d0);
    ls161_connect_qd(reg->ls161, reg->led[3], (void*)&indicator_in_d0);
    bitconst_connect_one(reg->ls161, (void*)&ls161_in_enp);
    bitconst_connect_one(reg->ls161, (void*)&ls161_in_ent);

    ls161_connect_qa(reg->ls161, reg->ls245_1, (void*)&ls245_in_a1);
    ls161_connect_qb(reg->ls161, reg->ls245_1, (void*)&ls245_in_a2);
    ls161_connect_qc(reg->ls161, reg->ls245_1, (void*)&ls245_in_a3);
    ls161_connect_qd(reg->ls161, reg->ls245_1, (void*)&ls245_in_a4);

    bitconst_connect_one(reg->ls245_1, (void*)&ls245_in_dir);

    if (name)
        strncpy(reg->name,name,sizeof(reg->name));
    else
        reg->name[0] = 0;

    reg->destroy = (void*)progctr_destroy;

    return reg;
}

////////////////////////////////////////////////////////////////////////////////
void progctr_destroy(progctr **dest){

    if (dest == NULL) return;
    progctr *b = *dest;
    if (b == NULL) return;

    part_destroy((void**)&b->ls161);
    part_destroy((void**)&b->ls245_1);
    part_destroy((void**)&b->ledclk);

    int i;
    for (i = 0; i < 4; i++)
        part_destroy((void**)&b->led[i]);

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void progctr_connect_bit_out (progctr *source, int index, void *dest, event_function_t dest_event_handler){

    switch(index){
        case 0:    ls245_connect_b1(source->ls245_1,dest,dest_event_handler); break;
        case 1:    ls245_connect_b2(source->ls245_1,dest,dest_event_handler); break;
        case 2:    ls245_connect_b3(source->ls245_1,dest,dest_event_handler); break;
        case 3:    ls245_connect_b4(source->ls245_1,dest,dest_event_handler); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_data_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, progctr *dest, int index){

    switch(index){
        case 0: connect_fn(from,dest->ls161,(void*)&ls161_in_da); break;
        case 1: connect_fn(from,dest->ls161,(void*)&ls161_in_db); break;
        case 2: connect_fn(from,dest->ls161,(void*)&ls161_in_dc); break;
        case 3: connect_fn(from,dest->ls161,(void*)&ls161_in_dd); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_clock(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_clk(dest->ls161, valptr, timestamp);
    indicator_in_d0(dest->ledclk, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_load(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_load(dest->ls161, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_clear(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_clear(dest->ls161, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_ct_enable(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_enp(dest->ls161, valptr, timestamp);
    ls161_in_ent(dest->ls161, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void progctr_in_oenable(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls245_in_oe(dest->ls245_1, valptr, timestamp);
}

void progctr_in_data0(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_da(dest->ls161, valptr, timestamp);
}

void progctr_in_data1(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_db(dest->ls161, valptr, timestamp);
}

void progctr_in_data2(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_dc(dest->ls161, valptr, timestamp);
}

void progctr_in_data3(progctr *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_dd(dest->ls161, valptr, timestamp);
}

event_function_t progctr_in_dataN[] = {

        (void*)progctr_in_data0,
        (void*)progctr_in_data1,
        (void*)progctr_in_data2,
        (void*)progctr_in_data3,
};

////////////////////////////////////////////////////////////////////////////////
board_object *progctr_board_create(progctr *reg, int key, char *name){

    board_object *board = board_create(40, 4, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 4; i++){

        j = 3-i;
        sprintf(s,"D%d",i);
        board_add_led(board, reg->led[i],1+4*j,1,s, LED_GREEN);
    }

    board_add_led(board, reg->ledclk,35,1,"CLK", LED_BLUE);

    return board;
}
