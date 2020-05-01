/*
 * reg_out.c
 *
 *  Created on: 01 de mai de 2020
 *      Author: milton
 */

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "reg_out.h"
#include "bitconst.h"

////////////////////////////////////////////////////////////////////////////////
reg_out *reg_out_create(char *name){

    reg_out *reg = malloc (sizeof(reg_out));
    if (!reg) return reg;

    char lshi[60];
    char lslo[60];

    lshi[0] = lslo[0] = 0;
    if (name){

        strncpy(lshi,name,sizeof(lshi));
        strncpy(lslo,name,sizeof(lslo));
    }

    strncat(lshi,"-hiWord",sizeof(lshi)/2);
    strncat(lslo,"-loWord",sizeof(lslo)/2);

    reg->ls173_hi = ls173_create(lshi);
    reg->ls173_lo = ls173_create(lslo);
    reg->ledclk = indicator_create("Clk");

    int i;
    for (i = 0; i < 8; i++){

        strncpy(lshi,name,sizeof(lshi));
        sprintf(lslo,"-D%d",i);
        strncat(lshi,lslo,sizeof(lshi)/2);
        reg->led[i] = indicator_create(lshi);
    }

    ls173_connect_1q(reg->ls173_lo, reg->led[0], (void*)&indicator_in_d0);
    ls173_connect_2q(reg->ls173_lo, reg->led[1], (void*)&indicator_in_d0);
    ls173_connect_3q(reg->ls173_lo, reg->led[2], (void*)&indicator_in_d0);
    ls173_connect_4q(reg->ls173_lo, reg->led[3], (void*)&indicator_in_d0);
    bitconst_connect_zero(reg->ls173_lo, (void*)&ls173_in_m);
    bitconst_connect_zero(reg->ls173_lo, (void*)&ls173_in_n);

    ls173_connect_1q(reg->ls173_hi, reg->led[4], (void*)&indicator_in_d0);
    ls173_connect_2q(reg->ls173_hi, reg->led[5], (void*)&indicator_in_d0);
    ls173_connect_3q(reg->ls173_hi, reg->led[6], (void*)&indicator_in_d0);
    ls173_connect_4q(reg->ls173_hi, reg->led[7], (void*)&indicator_in_d0);
    bitconst_connect_zero(reg->ls173_hi, (void*)&ls173_in_m);
    bitconst_connect_zero(reg->ls173_hi, (void*)&ls173_in_n);

    if (name)
        strncpy(reg->name,name,sizeof(reg->name));
    else
        reg->name[0] = 0;

    return reg;
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest, int index){

    switch(index){
        case 0: connect_fn(from,dest->ls173_lo,(void*)&ls173_in_1d); break;
        case 1: connect_fn(from,dest->ls173_lo,(void*)&ls173_in_2d); break;
        case 2: connect_fn(from,dest->ls173_lo,(void*)&ls173_in_3d); break;
        case 3: connect_fn(from,dest->ls173_lo,(void*)&ls173_in_4d); break;
        case 4: connect_fn(from,dest->ls173_hi,(void*)&ls173_in_1d); break;
        case 5: connect_fn(from,dest->ls173_hi,(void*)&ls173_in_2d); break;
        case 6: connect_fn(from,dest->ls173_hi,(void*)&ls173_in_3d); break;
        case 7: connect_fn(from,dest->ls173_hi,(void*)&ls173_in_4d); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_in_load_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_g1);
    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_g2);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_g1);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_g2);
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_in_clear_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_clr);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_clr);
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_in_clock_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, reg_out *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_clk);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_clk);
    connect_fn(from,dest->ledclk,(void*)&indicator_in_d0);
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_in_clock(reg_out *dest, int *valptr, int timestamp){

    ls173_in_clk(dest->ls173_lo, valptr, timestamp);
    ls173_in_clk(dest->ls173_hi, valptr, timestamp);
    indicator_in_d0(dest->ledclk, valptr, timestamp);
}

void reg_out_in_data0(reg_out *dest, int *valptr, int timestamp){

    ls173_in_1d(dest->ls173_lo, valptr, timestamp);
}

void reg_out_in_data1(reg_out *dest, int *valptr, int timestamp){

    ls173_in_2d(dest->ls173_lo, valptr, timestamp);
}

void reg_out_in_data2(reg_out *dest, int *valptr, int timestamp){

    ls173_in_3d(dest->ls173_lo, valptr, timestamp);
}

void reg_out_in_data3(reg_out *dest, int *valptr, int timestamp){

    ls173_in_4d(dest->ls173_lo, valptr, timestamp);
}

void reg_out_in_data4(reg_out *dest, int *valptr, int timestamp){

    ls173_in_1d(dest->ls173_hi, valptr, timestamp);
}

void reg_out_in_data5(reg_out *dest, int *valptr, int timestamp){

    ls173_in_2d(dest->ls173_hi, valptr, timestamp);
}

void reg_out_in_data6(reg_out *dest, int *valptr, int timestamp){

    ls173_in_3d(dest->ls173_hi, valptr, timestamp);
}

void reg_out_in_data7(reg_out *dest, int *valptr, int timestamp){

    ls173_in_4d(dest->ls173_hi, valptr, timestamp);
}


void (*reg_out_in_dataN[])(void *dest, int *valptr, int timestamp) = {

        (void*)reg_out_in_data0,
        (void*)reg_out_in_data1,
        (void*)reg_out_in_data2,
        (void*)reg_out_in_data3,
        (void*)reg_out_in_data4,
        (void*)reg_out_in_data5,
        (void*)reg_out_in_data6,
        (void*)reg_out_in_data7,
};

////////////////////////////////////////////////////////////////////////////////
board_object *reg_out_board_create(reg_out *reg, int key, char *name){

    board_object *board = board_create(40, 4, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 8; i++){

        j = 7-i;
        sprintf(s,"D%d",i);
        board_add_led(board, reg->led[i],1+4*j,1,s, LED_RED);
    }

    board_add_led(board, reg->ledclk,35,1,"CLK", LED_BLUE);

    return board;
}
