////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

// Based on Ben Eater's project (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "reg_8bit.h"
#include "bitconst.h"

////////////////////////////////////////////////////////////////////////////////
reg_8bit *reg_8bit_create(event_context_t *ec, reg8bit_type_t type, char *name){

    reg_8bit *reg = malloc (sizeof(reg_8bit));
    if (!reg) return reg;

    reg->ec = ec;

    char lshi[60];
    char lslo[60];

    lshi[0] = lslo[0] = 0;
    if (name){

        strncpy(lshi,name,sizeof(lshi));
        strncpy(lslo,name,sizeof(lslo));
    }

    strncat(lshi,"-hiWord",sizeof(lshi)/2);
    strncat(lslo,"-loWord",sizeof(lslo)/2);

    reg->ls173_hi = ls173_create(ec, lshi);
    reg->ls173_lo = ls173_create(ec, lslo);
    reg->ls245_1  = ls245_create(ec);
    reg->ledclk = indicator_create(ec, "Clk");

    reg->type = type;

    int i;
    for (i = 0; i < 8; i++){

        strncpy(lshi,name,sizeof(lshi));
        sprintf(lslo,"-D%d",i);
        strncat(lshi,lslo,sizeof(lshi)/2);
        reg->led[i] = indicator_create(ec, lshi);
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

    ls173_connect_1q(reg->ls173_lo, reg->ls245_1, (void*)&ls245_in_a1);
    ls173_connect_2q(reg->ls173_lo, reg->ls245_1, (void*)&ls245_in_a2);
    ls173_connect_3q(reg->ls173_lo, reg->ls245_1, (void*)&ls245_in_a3);
    ls173_connect_4q(reg->ls173_lo, reg->ls245_1, (void*)&ls245_in_a4);

    if (reg->type == REG8BIT_NORMAL){

        ls173_connect_1q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a5);
        ls173_connect_2q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a6);
        ls173_connect_3q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a7);
        ls173_connect_4q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a8);
    }

    bitconst_connect_one(reg->ls245_1, (void*)&ls245_in_dir);

    if (name)
        strncpy(reg->name,name,sizeof(reg->name));
    else
        reg->name[0] = 0;

    reg->destroy = (void*)reg_8bit_destroy;

    return reg;
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_destroy (reg_8bit **dest){

    if (dest == NULL) return;
    reg_8bit *b = *dest;
    if (b == NULL) return;

    ls173_destroy(&b->ls173_hi);
    ls173_destroy(&b->ls173_lo);
    ls245_destroy(&b->ls245_1);
    indicator_destroy(&b->ledclk);

    int i;
    for (i = 0; i < 8; i++)
        indicator_destroy(&b->led[i]);

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_connect_bit_out (reg_8bit *source, int index, void *dest, event_function_t dest_event_handler){

    switch(index){
        case 0:    ls245_connect_b1(source->ls245_1,dest,dest_event_handler); break;
        case 1:    ls245_connect_b2(source->ls245_1,dest,dest_event_handler); break;
        case 2:    ls245_connect_b3(source->ls245_1,dest,dest_event_handler); break;
        case 3:    ls245_connect_b4(source->ls245_1,dest,dest_event_handler); break;
        case 4:
            if (source->type == REG8BIT_NORMAL)
                ls245_connect_b5(source->ls245_1,dest,dest_event_handler);
            else
                ls173_connect_1q(source->ls173_hi, dest,dest_event_handler);
        break;
        case 5:
            if (source->type == REG8BIT_NORMAL)
                ls245_connect_b6(source->ls245_1,dest,dest_event_handler);
            else
                ls173_connect_2q(source->ls173_hi, dest,dest_event_handler);
        break;
        case 6:
            if (source->type == REG8BIT_NORMAL)
                ls245_connect_b7(source->ls245_1,dest,dest_event_handler);
            else
                ls173_connect_3q(source->ls173_hi, dest,dest_event_handler);
        break;
        case 7:
            if (source->type == REG8BIT_NORMAL)
                ls245_connect_b8(source->ls245_1,dest,dest_event_handler);
            else
                ls173_connect_4q(source->ls173_hi, dest,dest_event_handler);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_data_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_8bit *dest, int index){

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
void reg_8bit_in_load_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_8bit *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_g1);
    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_g2);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_g1);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_g2);
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_clear_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_8bit *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_clr);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_clr);
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_enable_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_8bit *dest){

    connect_fn(from,dest->ls245_1,(void*)&ls245_in_oe);
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_clock_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_8bit *dest){

    connect_fn(from,dest->ls173_lo,(void*)&ls173_in_clk);
    connect_fn(from,dest->ls173_hi,(void*)&ls173_in_clk);
    connect_fn(from,dest->ledclk,(void*)&indicator_in_d0);
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_clock(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_clk(dest->ls173_lo, valptr, timestamp);
    ls173_in_clk(dest->ls173_hi, valptr, timestamp);
    indicator_in_d0(dest->ledclk, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_in_enable(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls245_in_oe(dest->ls245_1, valptr, timestamp);
}

void reg_8bit_in_data0(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_1d(dest->ls173_lo, valptr, timestamp);
}

void reg_8bit_in_data1(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_2d(dest->ls173_lo, valptr, timestamp);
}

void reg_8bit_in_data2(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_3d(dest->ls173_lo, valptr, timestamp);
}

void reg_8bit_in_data3(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_4d(dest->ls173_lo, valptr, timestamp);
}

void reg_8bit_in_data4(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_1d(dest->ls173_hi, valptr, timestamp);
}

void reg_8bit_in_data5(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_2d(dest->ls173_hi, valptr, timestamp);
}

void reg_8bit_in_data6(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_3d(dest->ls173_hi, valptr, timestamp);
}

void reg_8bit_in_data7(reg_8bit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_4d(dest->ls173_hi, valptr, timestamp);
}


event_function_t reg_8bit_in_dataN[] = {

        (void*)reg_8bit_in_data0,
        (void*)reg_8bit_in_data1,
        (void*)reg_8bit_in_data2,
        (void*)reg_8bit_in_data3,
        (void*)reg_8bit_in_data4,
        (void*)reg_8bit_in_data5,
        (void*)reg_8bit_in_data6,
        (void*)reg_8bit_in_data7,
};

////////////////////////////////////////////////////////////////////////////////
board_object *reg_8bit_board_create(reg_8bit *reg, int key, char *name){

    board_object *board = board_create(40, 4, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 8; i++){

        j = 7-i;

        sprintf(s,"D%d",i);
        if (reg->type == REG8BIT_NORMAL)
            board_add_led(board, reg->led[i],1+4*j,1,s, LED_RED);
        else{
            if (i < 4)
                board_add_led(board, reg->led[i],1+4*j,1,s, LED_YELLOW);
            else
                board_add_led(board, reg->led[i],1+4*j,1,s, LED_BLUE);
        }
    }

    board_add_led(board, reg->ledclk,35,1,"CLK", LED_BLUE);

    return board;
}
