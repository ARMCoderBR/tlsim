/*
 * computer.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "computer.h"

#include "board.h"

#include "bitconst.h"
#include "indicator.h"
#include "ls173.h"
#include "ls245.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    ls173 *ls173_hi, *ls173_lo;
    ls245 *ls245_1;
    indicator *led[8];

} reg_8bit;

////////////////////////////////////////////////////////////////////////////////
void reg_8bit_init(reg_8bit *reg){

    reg->ls173_hi = ls173_create();
    reg->ls173_lo = ls173_create();
    reg->ls245_1  = ls245_create();

    int i;
    for (i = 0; i < 8; i++)
        reg->led[i] = indicator_create(NULL);

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

    ls173_connect_1q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a5);
    ls173_connect_2q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a6);
    ls173_connect_3q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a7);
    ls173_connect_4q(reg->ls173_hi, reg->ls245_1, (void*)&ls245_in_a8);

    bitconst_connect_one(reg->ls245_1, (void*)&ls245_in_dir);
}


////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    reg_8bit *reg_1 = malloc(sizeof(reg_8bit));

    reg_8bit_init(reg_1);

    bitswitch *sw_load = bitswitch_create();
    bitswitch *sw_clr = bitswitch_create();

    bitswitch *sw_enable = bitswitch_create();

    bitswitch_setval(sw_load,0);
    bitswitch_setval(sw_clr,1);

    bitswitch_setval(sw_enable,1);

    bitswitch *sw[8];
    indicator *oled[8];

    int i;
    for (i = 0; i < 8; i++){

        sw[i] = bitswitch_create();
        oled[i] = indicator_create(NULL);
    }

    bitswitch_connect_out(sw[0],reg_1->ls173_lo,(void*)&ls173_in_1d);
    bitswitch_connect_out(sw[1],reg_1->ls173_lo,(void*)&ls173_in_2d);
    bitswitch_connect_out(sw[2],reg_1->ls173_lo,(void*)&ls173_in_3d);
    bitswitch_connect_out(sw[3],reg_1->ls173_lo,(void*)&ls173_in_4d);
    bitswitch_connect_out(sw[4],reg_1->ls173_hi,(void*)&ls173_in_1d);
    bitswitch_connect_out(sw[5],reg_1->ls173_hi,(void*)&ls173_in_2d);
    bitswitch_connect_out(sw[6],reg_1->ls173_hi,(void*)&ls173_in_3d);
    bitswitch_connect_out(sw[7],reg_1->ls173_hi,(void*)&ls173_in_4d);

    ls245_connect_b1(reg_1->ls245_1,oled[0],(void*)&indicator_in_d0);
    ls245_connect_b2(reg_1->ls245_1,oled[1],(void*)&indicator_in_d0);
    ls245_connect_b3(reg_1->ls245_1,oled[2],(void*)&indicator_in_d0);
    ls245_connect_b4(reg_1->ls245_1,oled[3],(void*)&indicator_in_d0);
    ls245_connect_b5(reg_1->ls245_1,oled[4],(void*)&indicator_in_d0);
    ls245_connect_b6(reg_1->ls245_1,oled[5],(void*)&indicator_in_d0);
    ls245_connect_b7(reg_1->ls245_1,oled[6],(void*)&indicator_in_d0);
    ls245_connect_b8(reg_1->ls245_1,oled[7],(void*)&indicator_in_d0);

    bitswitch_connect_out(sw_load,reg_1->ls173_lo,(void*)&ls173_in_clk);
    bitswitch_connect_out(sw_load,reg_1->ls173_hi,(void*)&ls173_in_clk);

    bitswitch_connect_out(sw_clr, reg_1->ls173_lo, (void*)&ls173_in_clr);
    bitswitch_connect_out(sw_clr, reg_1->ls173_hi, (void*)&ls173_in_clr);

    bitswitch_connect_out(sw_enable,reg_1->ls245_1,(void*)&ls245_in_oe);

    board_init(0,0);

    char s[32];
    int j;

    for (i = 0; i < 8; i++){

        j = 7-i;
        sprintf(s,"In%d",j);
        board_add_led(reg_1->led[i],12*i,1,s);
        sprintf(s,"SW%d",j);
        board_add_manual_switch(sw[i], 12*i, 4, '0'+j, s);

        sprintf(s,"Data%d",j);
        board_add_led(reg_1->led[i],12*i,10,s);

        sprintf(s,"Out%d",j);
        board_add_led(oled[i],12*i,16,s);
    }

    board_add_manual_switch(sw_load, 0, 7, 'l', "Load");
    board_add_manual_switch(sw_clr, 12, 7, 'c', "Clr");

    board_add_manual_switch(sw_enable, 0, 13, 'e', "Enable");

//    board_assign_clock_to_switch(clk);
//
//    board_add_led(oclk,1,1,"CLK");
//
//    board_add_xdigit(ohex1,10,1,"COUNT");
//
//    board_add_led(omaxmin1,20,1,"MAXMIN");
//
//    board_add_led(oripclk1,30,1,"RIPPLE");
//
//    board_add_manual_switch(updownsel, 1, 4, KEY_F(1), "UP/DN");

    board_run();

}
