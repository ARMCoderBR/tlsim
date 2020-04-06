/*
 * computer.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <reg_8bit.h>

#include "computer.h"

#include "board.h"

#include "indicator.h"

////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    reg_8bit *reg_1 = malloc(sizeof(reg_8bit));

    reg_8bit_init(reg_1);

    bitswitch *sw_load = bitswitch_create();
    bitswitch *sw_clr = bitswitch_create();

    bitswitch *sw_enable = bitswitch_create();

    bitswitch *mainclk = bitswitch_create();
    indicator *indclk = indicator_create(NULL);
    bitswitch_connect_out(mainclk, indclk, (void*)&indicator_in_d0);

    bitswitch_setval(sw_load,0);
    bitswitch_setval(sw_clr,0);

    bitswitch_setval(sw_enable,1);

    bitswitch *sw[8];
    indicator *oled[8];

    int i;
    for (i = 0; i < 8; i++){

        sw[i] = bitswitch_create();
        oled[i] = indicator_create(NULL);
        reg_8bit_in_data((void*)&bitswitch_connect_out,sw[i],reg_1,i);
    }

//    bitswitch_connect_out(sw[0],reg_1->ls173_lo,(void*)&ls173_in_1d);
//    bitswitch_connect_out(sw[1],reg_1->ls173_lo,(void*)&ls173_in_2d);
//    bitswitch_connect_out(sw[2],reg_1->ls173_lo,(void*)&ls173_in_3d);
//    bitswitch_connect_out(sw[3],reg_1->ls173_lo,(void*)&ls173_in_4d);
//    bitswitch_connect_out(sw[4],reg_1->ls173_hi,(void*)&ls173_in_1d);
//    bitswitch_connect_out(sw[5],reg_1->ls173_hi,(void*)&ls173_in_2d);
//    bitswitch_connect_out(sw[6],reg_1->ls173_hi,(void*)&ls173_in_3d);
//    bitswitch_connect_out(sw[7],reg_1->ls173_hi,(void*)&ls173_in_4d);


    for (i = 0; i < 8; i++)
        reg_8bit_connect_bit_out (reg_1, i, oled[i], (void*)&indicator_in_d0);

//    ls245_connect_b1(reg_1->ls245_1,oled[0],(void*)&indicator_in_d0);
//    ls245_connect_b2(reg_1->ls245_1,oled[1],(void*)&indicator_in_d0);
//    ls245_connect_b3(reg_1->ls245_1,oled[2],(void*)&indicator_in_d0);
//    ls245_connect_b4(reg_1->ls245_1,oled[3],(void*)&indicator_in_d0);
//    ls245_connect_b5(reg_1->ls245_1,oled[4],(void*)&indicator_in_d0);
//    ls245_connect_b6(reg_1->ls245_1,oled[5],(void*)&indicator_in_d0);
//    ls245_connect_b7(reg_1->ls245_1,oled[6],(void*)&indicator_in_d0);
//    ls245_connect_b8(reg_1->ls245_1,oled[7],(void*)&indicator_in_d0);

    reg_8bit_in_load((void*)&bitswitch_connect_out,sw_load,reg_1);
    //bitswitch_connect_out(sw_load,reg_1->ls173_lo,(void*)&ls173_in_g1);
    //bitswitch_connect_out(sw_load,reg_1->ls173_lo,(void*)&ls173_in_g2);
    //bitswitch_connect_out(sw_load,reg_1->ls173_hi,(void*)&ls173_in_g1);
    //bitswitch_connect_out(sw_load,reg_1->ls173_hi,(void*)&ls173_in_g2);

    reg_8bit_in_clear((void*)&bitswitch_connect_out,sw_clr,reg_1);
    //bitswitch_connect_out(sw_clr, reg_1->ls173_lo, (void*)&ls173_in_clr);
    //bitswitch_connect_out(sw_clr, reg_1->ls173_hi, (void*)&ls173_in_clr);

    reg_8bit_in_enable((void*)&bitswitch_connect_out,sw_enable,reg_1);
    //bitswitch_connect_out(sw_enable,reg_1->ls245_1,(void*)&ls245_in_oe);

    reg_8bit_in_clock((void*)&bitswitch_connect_out,mainclk,reg_1);
    //bitswitch_connect_out(mainclk, reg_1->ls173_lo, (void*)&ls173_in_clk);
    //bitswitch_connect_out(mainclk, reg_1->ls173_hi, (void*)&ls173_in_clk);

    board_init(0,0);

    char s[32];
    int j;

    for (i = 0; i < 8; i++){

        j = 7-i;
        sprintf(s,"SW%d",j);
        board_add_manual_switch(sw[i], 12*i, 4, '0'+j, s);

        sprintf(s,"Data%d",j);
        board_add_led(reg_1->led[i],12*i,10,s);

        sprintf(s,"Out%d",j);
        board_add_led(oled[i],12*i,16,s);
    }

    board_add_manual_switch(sw_load, 0, 7, 'l', "Load");
    board_add_manual_switch(sw_clr, 12, 7, 'c', "Clr");
    board_add_led(indclk, 24, 7, "Clk");

    board_add_manual_switch(sw_enable, 0, 13, 'e', "Enable");

    board_assign_clock_to_switch(mainclk);


    board_run();

}
