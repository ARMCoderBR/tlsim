/*
 * computer.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include "computer.h"

#include "board.h"







void computer_sim(){















    board_init(0,0);

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
