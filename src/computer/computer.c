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

    reg_8bit *reg_1 = reg_8bit_create();
    reg_8bit *reg_2 = reg_8bit_create();
    reg_8bit *reg_3 = reg_8bit_create();

    if ((!reg_1)||(!reg_2)||(!reg_3)){

        perror("reg_x create");
        exit(0);
    }

    board_object *mainboard = board_create(0,0,0,"COMPUTER");

    board_object *reg1_board = reg_8bit_board_create(reg_1, KEY_F(1), "Reg1"); // Requer NCURSES
    board_object *reg2_board = reg_8bit_board_create(reg_2, KEY_F(2), "Reg2"); // Requer NCURSES
    board_object *reg3_board = reg_8bit_board_create(reg_3, KEY_F(3), "Reg3"); // Requer NCURSES

    if ((!reg1_board)||(!reg2_board)||(!reg3_board)){

        perror("regx_board create");
        exit(0);
    }

    board_clock_connect(reg_1, (void*)&reg_8bit_in_clock);
    board_clock_connect(reg_2, (void*)&reg_8bit_in_clock);
    board_clock_connect(reg_3, (void*)&reg_8bit_in_clock);

    board_add_board(mainboard,reg1_board,1,1);
    board_add_board(mainboard,reg2_board,1,5);
    board_add_board(mainboard,reg3_board,1,9);


    bitswitch *sw_clr = bitswitch_create();
    bitswitch_setval(sw_clr, 0);

    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,reg_1);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,reg_2);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,reg_3);

    board_add_manual_switch(mainboard, sw_clr, 2, 13, 'c', "Clr");


    bitswitch *sw_load1 = bitswitch_create();
    bitswitch *sw_load2 = bitswitch_create();
    bitswitch *sw_load3 = bitswitch_create();

    bitswitch *sw_enable1 = bitswitch_create();
    bitswitch *sw_enable2 = bitswitch_create();
    bitswitch *sw_enable3 = bitswitch_create();



    bitswitch_setval(sw_load1, 1);
    bitswitch_setval(sw_enable1, 1);

    bitswitch_setval(sw_load2, 1);
    bitswitch_setval(sw_enable2, 1);

    bitswitch_setval(sw_load3, 1);
    bitswitch_setval(sw_enable3, 1);

    bitswitch_setval(sw_clr, 0);

    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load1,reg_1);
    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load2,reg_2);
    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load3,reg_3);

    board_add_manual_switch(mainboard, sw_load1, 44, 2, '1', "Load1");
    board_add_manual_switch(mainboard, sw_load2, 44, 6, '2', "Load2");
    board_add_manual_switch(mainboard, sw_load3, 44, 10, '3', "Load3");


    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable1,reg_1);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable2,reg_2);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable3,reg_3);

    board_add_manual_switch(mainboard, sw_enable1, 55, 2, 'q', "EN1");
    board_add_manual_switch(mainboard, sw_enable2, 55, 6, 'w', "EN2");
    board_add_manual_switch(mainboard, sw_enable3, 55, 10, 'e', "EN3");


    board_run(mainboard);
}
