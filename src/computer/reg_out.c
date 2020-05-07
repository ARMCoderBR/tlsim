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

    unsigned char bufcreate[2048];
    memset(bufcreate,0xff,sizeof(bufcreate));

    for (i = 0; i <= 255; i++){
        bufcreate[i] = map7seg(i % 10);
        bufcreate[256+i] = map7seg((i/10) % 10);
        bufcreate[512+i] = map7seg((i/100) % 10);
        bufcreate[768+i] = 0;
    }

    for (i = -128; i <= 127; i++){
        int ii = abs(i);
        int addr;
        if (i >= 0)
            addr = 1024 + i;
        else
            addr = 1024 + 256 + i;
        bufcreate[addr] = map7seg(ii % 10);
        bufcreate[256+addr] = map7seg((ii/10) % 10);
        bufcreate[512+addr] = map7seg((ii/100) % 10);
        if (i < 0)
            bufcreate[768+addr] = MSK_G;
        else
            bufcreate[768+addr] = 0;
    }

    reg->eep1 = at28c16_create("",bufcreate);

    for (i = 0; i < 4; i++){

        reg->display[i] = dis7seg_create(COMMON_K,"");

        //bitconst_connect_zero(reg->display[i], (void*)&dis7seg_in_common);
        at28c16_connect_o7(reg->eep1, reg->display[i], (void*)&dis7seg_in_segdp);
        at28c16_connect_o6(reg->eep1, reg->display[i], (void*)&dis7seg_in_sega);
        at28c16_connect_o5(reg->eep1, reg->display[i], (void*)&dis7seg_in_segb);
        at28c16_connect_o4(reg->eep1, reg->display[i], (void*)&dis7seg_in_segc);
        at28c16_connect_o3(reg->eep1, reg->display[i], (void*)&dis7seg_in_segd);
        at28c16_connect_o2(reg->eep1, reg->display[i], (void*)&dis7seg_in_sege);
        at28c16_connect_o1(reg->eep1, reg->display[i], (void*)&dis7seg_in_segf);
        at28c16_connect_o0(reg->eep1, reg->display[i], (void*)&dis7seg_in_segg);
    }

//    bitconst_connect_zero(reg->eep1, (void*)&at28c16_in_a8);
//    bitconst_connect_zero(reg->eep1, (void*)&at28c16_in_a9);
    //bitconst_connect_zero(reg->eep1, (void*)&at28c16_in_a10);

    bitconst_connect_one(reg->eep1, (void*)&at28c16_in_we);
    bitconst_connect_zero(reg->eep1, (void*)&at28c16_in_cs);
    bitconst_connect_zero(reg->eep1, (void*)&at28c16_in_oe);

    ls173_connect_1q(reg->ls173_lo, reg->led[0], (void*)&indicator_in_d0);
    ls173_connect_2q(reg->ls173_lo, reg->led[1], (void*)&indicator_in_d0);
    ls173_connect_3q(reg->ls173_lo, reg->led[2], (void*)&indicator_in_d0);
    ls173_connect_4q(reg->ls173_lo, reg->led[3], (void*)&indicator_in_d0);
    ls173_connect_1q(reg->ls173_lo, reg->eep1, (void*)&at28c16_in_a0);
    ls173_connect_2q(reg->ls173_lo, reg->eep1, (void*)&at28c16_in_a1);
    ls173_connect_3q(reg->ls173_lo, reg->eep1, (void*)&at28c16_in_a2);
    ls173_connect_4q(reg->ls173_lo, reg->eep1, (void*)&at28c16_in_a3);

    bitconst_connect_zero(reg->ls173_lo, (void*)&ls173_in_m);
    bitconst_connect_zero(reg->ls173_lo, (void*)&ls173_in_n);

    ls173_connect_1q(reg->ls173_hi, reg->led[4], (void*)&indicator_in_d0);
    ls173_connect_2q(reg->ls173_hi, reg->led[5], (void*)&indicator_in_d0);
    ls173_connect_3q(reg->ls173_hi, reg->led[6], (void*)&indicator_in_d0);
    ls173_connect_4q(reg->ls173_hi, reg->led[7], (void*)&indicator_in_d0);
    ls173_connect_1q(reg->ls173_hi, reg->eep1, (void*)&at28c16_in_a4);
    ls173_connect_2q(reg->ls173_hi, reg->eep1, (void*)&at28c16_in_a5);
    ls173_connect_3q(reg->ls173_hi, reg->eep1, (void*)&at28c16_in_a6);
    ls173_connect_4q(reg->ls173_hi, reg->eep1, (void*)&at28c16_in_a7);

    bitconst_connect_zero(reg->ls173_hi, (void*)&ls173_in_m);
    bitconst_connect_zero(reg->ls173_hi, (void*)&ls173_in_n);

    if (name)
        strncpy(reg->name,name,sizeof(reg->name));
    else
        reg->name[0] = 0;

    reg->clk = clkgen_create("");
    reg->ledclki = indicator_create("");

    clkgen_connect_out(reg->clk, reg->ledclki, (void*)&indicator_in_d0);

    reg->ls76 = ls76_create();
    reg->led76_0 = indicator_create("");
    reg->led76_1 = indicator_create("");
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_1j);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_1k);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_1pre);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_1clr);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_2j);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_2k);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_2pre);
    bitconst_connect_one(reg->ls76,(void*)&ls76_in_2clr);
    ls76_connect_1qn(reg->ls76, reg->ls76, (void*)&ls76_in_2clk);
    ls76_connect_1q(reg->ls76, reg->led76_0, (void*)&indicator_in_d0);
    ls76_connect_2q(reg->ls76, reg->led76_1, (void*)&indicator_in_d0);

    clkgen_connect_out(reg->clk, reg->ls76, (void*)&ls76_in_1clk);

    reg->ls139 = ls139_create("");
    bitconst_connect_zero(reg->ls139,(void*)&ls139_in_1g);
    ls76_connect_1q(reg->ls76, reg->ls139, (void*)&ls139_in_1a);
    ls76_connect_2q(reg->ls76, reg->ls139, (void*)&ls139_in_1b);

    ls76_connect_1q(reg->ls76, reg->eep1, (void*)&at28c16_in_a8);
    ls76_connect_2q(reg->ls76, reg->eep1, (void*)&at28c16_in_a9);


    ls139_connect_1y0(reg->ls139,reg->display[3], (void*)&dis7seg_in_common);
    ls139_connect_1y1(reg->ls139,reg->display[2], (void*)&dis7seg_in_common);
    ls139_connect_1y2(reg->ls139,reg->display[1], (void*)&dis7seg_in_common);
    ls139_connect_1y3(reg->ls139,reg->display[0], (void*)&dis7seg_in_common);

    reg->sw_signed = bitswitch_create("Un/Sg");

    bitswitch_connect_out(reg->sw_signed, reg->eep1, (void*)&at28c16_in_a10);

    reg->destroy = (void*)reg_out_destroy;

    return reg;
}

////////////////////////////////////////////////////////////////////////////////
void reg_out_destroy (reg_out **dest){

    if (dest == NULL) return;
    reg_out *b = *dest;
    if (b == NULL) return;

    ls173_destroy(&b->ls173_hi);
    ls173_destroy(&b->ls173_lo);
    indicator_destroy(&b->ledclk);

    int i;
    for (i = 0; i < 8; i++)
        indicator_destroy(&b->led[i]);

    at28c16_destroy(&b->eep1);

    for (i = 0; i < 4; i++)
        dis7seg_destroy(&b->display[i]);

    clkgen_destroy(&b->clk);
    indicator_destroy(&b->ledclki);

    ls76_destroy(&b->ls76);
    indicator_destroy(&b->led76_0);
    indicator_destroy(&b->led76_1);

    ls139_destroy(&b->ls139);
    bitswitch_destroy(&b->sw_signed);


    free(b);
    *dest = NULL;
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

    board_object *board = board_create(40, 7, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 8; i++){

        j = 7-i;
        sprintf(s,"D%d",i);
        board_add_led(board, reg->led[i],1+4*j,1,s, LED_RED);
    }

    board_add_led(board, reg->ledclk,35,1,"CLK", LED_BLUE);

//    board_add_led(board, reg->ledclki,1,4,"CKi", LED_YELLOW);
//    board_add_led(board, reg->led76_1,5,4,"C1", LED_GREEN);
//    board_add_led(board, reg->led76_0,9,4,"C0", LED_GREEN);

    board_add_manual_switch(board, reg->sw_signed,3,4,'s', "Un/Sg");

    board_add_display_7seg(board, reg->display[0],14,3,"DS0", LED_RED);
    board_add_display_7seg(board, reg->display[1],19,3,"DS1", LED_RED);
    board_add_display_7seg(board, reg->display[2],24,3,"DS2", LED_RED);
    board_add_display_7seg(board, reg->display[3],29,3,"DS3", LED_RED);

    return board;
}
