/*
 * computer.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "reg_8bit.h"
#include "alu_8bit.h"

#include "computer.h"

#include "board.h"

#include "indicator.h"

////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    reg_8bit *regA = reg_8bit_create();
    reg_8bit *regB = reg_8bit_create();
    reg_8bit *regIN = reg_8bit_create();

    alu_8bit *alu = alu_8bit_create();

    if ((!regA)||(!regB)||(!regIN)){

        perror("reg_x create");
        exit(0);
    }

    board_object *mainboard = board_create(0,0,0,"COMPUTER");

    board_object *regA_board = reg_8bit_board_create(regA, KEY_F(1), "Reg A"); // Requer NCURSES
    board_object *regB_board = reg_8bit_board_create(regB, KEY_F(2), "Reg B"); // Requer NCURSES
    board_object *regIN_board = reg_8bit_board_create(regIN, KEY_F(3), "Reg IN"); // Requer NCURSES

    board_object *alu_board = alu_8bit_board_create(alu, KEY_F(4), "ALU"); // Requer NCURSES

    if ((!regA_board)||(!regB_board)||(!regIN_board)||(!alu_board)){

        perror("regx_board create");
        exit(0);
    }

    board_clock_connect(regA, (void*)&reg_8bit_in_clock);
    board_clock_connect(regB, (void*)&reg_8bit_in_clock);
    board_clock_connect(regIN, (void*)&reg_8bit_in_clock);

    board_add_board(mainboard,regA_board,1,1);
    board_add_board(mainboard,alu_board,1,5);
    board_add_board(mainboard,regB_board,1,9);
    board_add_board(mainboard,regIN_board,1,13);

    bitswitch *sw_clr = bitswitch_create();
    bitswitch_setval(sw_clr, 0);

    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regA);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regB);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regIN);

    board_add_manual_switch(mainboard, sw_clr, 2, 17, 'c', "Clr");

    bitswitch *sw_load1 = bitswitch_create();
    bitswitch *sw_load2 = bitswitch_create();
    bitswitch *sw_load3 = bitswitch_create();

    bitswitch *sw_enable1 = bitswitch_create();
    bitswitch *sw_enable2 = bitswitch_create();
    bitswitch *sw_enable3 = bitswitch_create();

    bitswitch *sw_sub = bitswitch_create();
    bitswitch *sw_eo = bitswitch_create();


    bitswitch_setval(sw_load1, 1);
    bitswitch_setval(sw_enable1, 1);

    bitswitch_setval(sw_load2, 1);
    bitswitch_setval(sw_enable2, 1);

    bitswitch_setval(sw_load3, 1);
    bitswitch_setval(sw_enable3, 1);

    bitswitch_setval(sw_clr, 0);

    bitswitch_setval(sw_eo, 1);

    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load1,regA);
    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load2,regB);
    reg_8bit_in_load_from((void*)&bitswitch_connect_out,sw_load3,regIN);

    board_add_manual_switch(mainboard, sw_load1, 44, 2, 'q', "Load1");
    board_add_manual_switch(mainboard, sw_load2, 44, 4+6, 'a', "Load2");
    board_add_manual_switch(mainboard, sw_load3, 44, 4+10, 'z', "Load3");

    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable1,regA);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable2,regB);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable3,regIN);

    board_add_manual_switch(mainboard, sw_enable1, 55, 2, 'w', "EN1");
    board_add_manual_switch(mainboard, sw_enable2, 55, 4+6, 's', "EN2");
    board_add_manual_switch(mainboard, sw_enable3, 55, 4+10, 'x', "EN3");

    indicator *ledbus[8];
    bitswitch *swbus[8];

    ls173_connect_1q(regA->ls173_lo, alu, alu_8bit_in_dataAN[0]);
    ls173_connect_2q(regA->ls173_lo, alu, alu_8bit_in_dataAN[1]);
    ls173_connect_3q(regA->ls173_lo, alu, alu_8bit_in_dataAN[2]);
    ls173_connect_4q(regA->ls173_lo, alu, alu_8bit_in_dataAN[3]);
    ls173_connect_1q(regA->ls173_hi, alu, alu_8bit_in_dataAN[4]);
    ls173_connect_2q(regA->ls173_hi, alu, alu_8bit_in_dataAN[5]);
    ls173_connect_3q(regA->ls173_hi, alu, alu_8bit_in_dataAN[6]);
    ls173_connect_4q(regA->ls173_hi, alu, alu_8bit_in_dataAN[7]);

    ls173_connect_1q(regB->ls173_lo, alu, alu_8bit_in_dataBN[0]);
    ls173_connect_2q(regB->ls173_lo, alu, alu_8bit_in_dataBN[1]);
    ls173_connect_3q(regB->ls173_lo, alu, alu_8bit_in_dataBN[2]);
    ls173_connect_4q(regB->ls173_lo, alu, alu_8bit_in_dataBN[3]);
    ls173_connect_1q(regB->ls173_hi, alu, alu_8bit_in_dataBN[4]);
    ls173_connect_2q(regB->ls173_hi, alu, alu_8bit_in_dataBN[5]);
    ls173_connect_3q(regB->ls173_hi, alu, alu_8bit_in_dataBN[6]);
    ls173_connect_4q(regB->ls173_hi, alu, alu_8bit_in_dataBN[7]);

    int i;
    for (i = 0; i < 8; i++){

        ledbus[i] = indicator_create("Data");

        swbus[i] = bitswitch_create();
        bitswitch_setval(swbus[i], 1);

        bitswitch_connect_out(swbus[i], ledbus[i], (void*)indicator_in_d0);

        bitswitch_connect_out(swbus[i], regA, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regB, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regIN, (void*)reg_8bit_in_dataN[i]);

        reg_8bit_connect_bit_out (regA, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regB, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regIN, i, ledbus[i], (void*)indicator_in_d0);
        alu_8bit_connect_bit_out (alu, i, ledbus[i], (void*)indicator_in_d0);

        reg_8bit_connect_bit_out (regA, i, regB, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, regIN, reg_8bit_in_dataN[i]);

        reg_8bit_connect_bit_out (regB, i, regA, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, regIN, reg_8bit_in_dataN[i]);

        reg_8bit_connect_bit_out (regIN, i, regA, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regIN, i, regB, reg_8bit_in_dataN[i]);

        alu_8bit_connect_bit_out (alu, i, regA, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regB, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regIN, reg_8bit_in_dataN[i]);

        int j = 7-i;

        board_add_led(mainboard,ledbus[i],1+8*i, 4+15, "Dn");

        board_add_manual_switch(mainboard, swbus[i], 1+8*i, 4+18, '0'+j, "Dn");
    }

    board_add_manual_switch(mainboard, sw_sub, 44, 6, 'S', "SUB");
    board_add_manual_switch(mainboard, sw_eo, 55, 6, 'E', "EO");

    alu_8bit_in_sub_from((void*)&bitswitch_connect_out, sw_sub, alu);
    alu_8bit_in_enable_from((void*)&bitswitch_connect_out, sw_eo, alu);

    board_run(mainboard);
}
