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
#include "ram_8bit.h"
#include "progctr.h"
#include "bitconst.h"
#include "ctrunit.h"

#include "computer.h"

#include "board.h"

#include "indicator.h"

////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    board_object *mainboard = board_create(0,0,0,"BEN EATER'S 8-BIT COMPUTER");


    //////// REGS //////////////////////////////////////////////////////////////

    reg_8bit *regA = reg_8bit_create(REG8BIT_NORMAL, "regA");
    reg_8bit *regB = reg_8bit_create(REG8BIT_NORMAL, "regB");
    reg_8bit *regIN = reg_8bit_create(REG8BIT_IR, "regIN");

    if ((!regA)||(!regB)||(!regIN)){

        perror("reg_x create");
        exit(0);
    }

    board_object *regA_board = reg_8bit_board_create(regA, KEY_F(1), "Reg A"); // Requer NCURSES
    board_object *regB_board = reg_8bit_board_create(regB, KEY_F(2), "Reg B"); // Requer NCURSES
    board_object *regIN_board = reg_8bit_board_create(regIN, KEY_F(3), "Reg IN"); // Requer NCURSES

    if ((!regA_board)||(!regB_board)||(!regIN_board)){

        perror("regx_board create");
        exit(0);
    }

    board_clock_connect(regA, (void*)&reg_8bit_in_clock);
    board_clock_connect(regB, (void*)&reg_8bit_in_clock);
    board_clock_connect(regIN, (void*)&reg_8bit_in_clock);

    board_add_board(mainboard,regA_board,1,1);
    board_add_board(mainboard,regB_board,1,9);
    board_add_board(mainboard,regIN_board,1,13);

    bitswitch *sw_clr = bitswitch_create("CLR");
    bitswitch_setval(sw_clr, 0);

    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regA);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regB);
    reg_8bit_in_clear_from((void*)&bitswitch_connect_out,sw_clr,regIN);

    board_add_manual_switch(mainboard, sw_clr, 2, 17, 'c', "Clr");

    bitswitch *sw_load1 = bitswitch_create("LOADA");
    bitswitch *sw_load2 = bitswitch_create("LOADB");
    bitswitch *sw_load3 = bitswitch_create("LOADIN");

    bitswitch *sw_enable1 = bitswitch_create("ENA");
    bitswitch *sw_enable2 = bitswitch_create("ENB");
    bitswitch *sw_enable3 = bitswitch_create("ENIN");

    bitswitch *sw_sub = bitswitch_create("SUB");
    bitswitch *sw_eo = bitswitch_create("EO");

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

    board_add_manual_switch(mainboard, sw_load1, 42, 2, 'q', "Load1");
    board_add_manual_switch(mainboard, sw_load2, 42, 4+6, 'a', "Load2");
    board_add_manual_switch(mainboard, sw_load3, 42, 4+10, 'z', "Load3");

    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable1,regA);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable2,regB);
    reg_8bit_in_enable_from((void*)&bitswitch_connect_out,sw_enable3,regIN);

    board_add_manual_switch(mainboard, sw_enable1, 53, 2, 'w', "EN1");
    board_add_manual_switch(mainboard, sw_enable2, 53, 4+6, 's', "EN2");
    board_add_manual_switch(mainboard, sw_enable3, 53, 4+10, 'x', "EN3");


    //////// ALU ///////////////////////////////////////////////////////////////

    alu_8bit *alu = alu_8bit_create("ALU");
    board_object *alu_board = alu_8bit_board_create(alu, KEY_F(4), "ALU"); // Requer NCURSES
    board_add_board(mainboard,alu_board,1,5);

    board_add_manual_switch(mainboard, sw_sub, 42, 6, 'S', "SUB");
    board_add_manual_switch(mainboard, sw_eo, 53, 6, 'E', "EO");

    alu_8bit_in_sub_from((void*)&bitswitch_connect_out, sw_sub, alu);
    alu_8bit_in_enable_from((void*)&bitswitch_connect_out, sw_eo, alu);

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


    //////// RAM ///////////////////////////////////////////////////////////////

    ram_8bit *ram = ram_8bit_create("RAM");
    board_object *ram_board = ram_8bit_board_create(ram, KEY_F(5), "RAM"); // Requer NCURSES
    board_add_board(mainboard,ram_board,66,1);
    board_clock_connect(ram, (void*)&ram_8bit_in_clk);

    bitswitch *ram_oe = bitswitch_create("RAMOE");
    bitswitch *ram_waddr = bitswitch_create("RAMWA");
    bitswitch *ram_wdata = bitswitch_create("RAMWD");

    bitswitch_setval(ram_oe, 1);
    bitswitch_setval(ram_waddr, 1);

    bitswitch_connect_out(ram_oe, ram, (void*)&ram_8bit_in_oe);
    bitswitch_connect_out(ram_waddr, ram, (void*)&ram_8bit_in_waddr);
    bitswitch_connect_out(ram_wdata, ram, (void*)&ram_8bit_in_wdata);

    board_add_manual_switch(mainboard, ram_oe, 66, 14, 'j', "OE");
    board_add_manual_switch(mainboard, ram_waddr, 74, 14, 'k', "WADD");
    board_add_manual_switch(mainboard, ram_wdata, 82, 14, 'l', "WDAT");


    //////// PROGRAM COUNTER ///////////////////////////////////////////////////

    progctr *pctr = progctr_create("PC");
    board_object *pctr_board = progctr_board_create(pctr, KEY_F(6), "PC");
    board_add_board(mainboard,pctr_board,1,26);
    board_clock_connect(pctr, (void*)&progctr_in_clock);

    bitswitch *prctr_cten = bitswitch_create("CTEN");
    bitswitch *prctr_eo = bitswitch_create("EO");
    bitswitch *prctr_jmp = bitswitch_create("JMP");

    bitswitch_setval(prctr_cten,1);
    bitswitch_setval(prctr_eo,1);
    bitswitch_setval(prctr_jmp,1);

    bitswitch_connect_out(prctr_cten, pctr, (void*)&progctr_in_ct_enable);
    bitswitch_connect_out(prctr_eo, pctr, (void*)&progctr_in_oenable);
    bitswitch_connect_out(prctr_jmp, pctr, (void*)&progctr_in_load);
    bitconst_connect_one(pctr,(void*)&progctr_in_clear);

    board_add_manual_switch(mainboard, prctr_cten, 42, 27, 'b', "CTEn");
    board_add_manual_switch(mainboard, prctr_eo, 50, 27, 'n', "EO");
    board_add_manual_switch(mainboard, prctr_jmp, 58, 27, 'm', "JMP");


    //////// BUS ///////////////////////////////////////////////////////////////

    indicator *ledbus[8];
    bitswitch *swbus[8];

    int i;
    for (i = 0; i < 8; i++){

        ledbus[i] = indicator_create("Data");

        char dname[10];
        sprintf(dname,"D%d",i);

        swbus[i] = bitswitch_create(dname);
        bitswitch_setval(swbus[i], 1);

        /// BUS TEST SWITCHES
        bitswitch_connect_out(swbus[i], ledbus[i], (void*)indicator_in_d0);
        bitswitch_connect_out(swbus[i], regA, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regB, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regIN, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], ram, (void*)ram_8bit_in_dataN[i]);

        /// REGA OUTPUT
        reg_8bit_connect_bit_out (regA, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regA, i, regB, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, ram, ram_8bit_in_dataN[i]);

        /// REGB OUTPUT
        reg_8bit_connect_bit_out (regB, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regB, i, regA, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, ram, ram_8bit_in_dataN[i]);

        /// REGIN OUTPUT
        if (i < 4){

            reg_8bit_connect_bit_out (regIN, i, ledbus[i], (void*)indicator_in_d0);
            reg_8bit_connect_bit_out (regIN, i, regA, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (regIN, i, regB, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (regIN, i, ram, ram_8bit_in_dataN[i]);
        }

        /// ALU OUTPUT
        alu_8bit_connect_bit_out (alu, i, ledbus[i], (void*)indicator_in_d0);
        alu_8bit_connect_bit_out (alu, i, regA, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regB, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regIN, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, ram, ram_8bit_in_dataN[i]);

        /// RAM OUTPUT
        ram_8bit_connect_bit_out(ram, i, ledbus[i], (void*)indicator_in_d0);
        ram_8bit_connect_bit_out (ram, i, regA, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, regB, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, regIN, reg_8bit_in_dataN[i]);

        int j = 7-i;

        board_add_led(mainboard,ledbus[i],2+7*j, 4+15, dname, LED_RED);

        board_add_manual_switch(mainboard, swbus[i], 2+7*j, 4+18, '0'+i, dname);
    }

    for (i = 0; i < 4; i++){

        reg_8bit_connect_bit_out (regA, i, pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (regIN, i, pctr, progctr_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, pctr, progctr_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, pctr, progctr_in_dataN[i]);

        bitswitch_connect_out(swbus[i], pctr, (void*)progctr_in_dataN[i]);

        progctr_connect_bit_out (pctr, i, ledbus[i], (void*)indicator_in_d0);
        progctr_connect_bit_out (pctr, i, regA, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, regB, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, regIN, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, ram, ram_8bit_in_dataN[i]);

    }


    //////// CONTROL UNIT //////////////////////////////////////////////////////

    ctrunit *ctru =ctrunit_create("CONTROL UNIT");
    board_object *ctru_board = ctrunit_board_create(ctru, '*', "CONTROL UNIT");
    board_add_board(mainboard,ctru_board,1,32);

    board_run(mainboard);
}
