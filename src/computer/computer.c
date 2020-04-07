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

    board_object *mainboard = board_create(0,0,0,"COMPUTER");
    board_object *reg1_board = reg_8bit_board_create(reg_1, KEY_F(1), "Reg1"); // Requer NCURSES
    board_add_board(mainboard,reg1_board,1,1,0,NULL);

    //board_assign_clock_to_switch(mainclk);


    board_run(mainboard);

}
