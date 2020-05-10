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
#include "reg_out.h"
#include "computer.h"
#include "board.h"
#include "indicator.h"
#include "clkgen.h"

////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    board_object *mainboard = board_create(0,0,0,"BEN EATER'S 8-BIT COMPUTER");

    clkgen *mainclk = clkgen_create("",1000000);

    bitconst_connect_zero(mainclk, (void*)&clkgen_in_halt);

    board_set_clk(mainclk);

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

//    board_clock_connect(regA, (void*)&reg_8bit_in_clock);
//    board_clock_connect(regB, (void*)&reg_8bit_in_clock);
//    board_clock_connect(regIN, (void*)&reg_8bit_in_clock);

    clkgen_connect_out(mainclk, regA, (void*)&reg_8bit_in_clock);
    clkgen_connect_out(mainclk, regB, (void*)&reg_8bit_in_clock);
    clkgen_connect_out(mainclk, regIN, (void*)&reg_8bit_in_clock);

    board_add_board(mainboard,regA_board,1,1);
    board_add_board(mainboard,regB_board,1,9);
    board_add_board(mainboard,regIN_board,1,13);


    //////// ALU ///////////////////////////////////////////////////////////////

    alu_8bit *alu = alu_8bit_create("ALU");
    board_object *alu_board = alu_8bit_board_create(alu, KEY_F(4), "ALU"); // Requer NCURSES
    board_add_board(mainboard,alu_board,1,5);

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
    //board_clock_connect(ram, (void*)&ram_8bit_in_clk);
    clkgen_connect_out(mainclk, ram, (void*)&ram_8bit_in_clk);

    //////// PROGRAM COUNTER ///////////////////////////////////////////////////

    progctr *pctr = progctr_create("PC");
    board_object *pctr_board = progctr_board_create(pctr, KEY_F(6), "PC");
    board_add_board(mainboard,pctr_board,1,26);
    //board_clock_connect(pctr, (void*)&progctr_in_clock);
    clkgen_connect_out(mainclk, pctr, (void*)&progctr_in_clock);

    //////// REG OUT ///////////////////////////////////////////////////////////

    reg_out *regout = reg_out_create("RO");
    board_object *regout_board = reg_out_board_create(regout, KEY_F(6), "RO");
    board_add_board(mainboard,regout_board,66,18);
    //board_clock_connect(regout, (void*)&reg_out_in_clock);
    clkgen_connect_out(mainclk, regout, (void*)&reg_out_in_clock);

    //////// BUS ///////////////////////////////////////////////////////////////

    indicator *ledbus[8];
    board_object *bus_board = board_create(40, 4, 0, "BUS");

#if DISABLE_CTRUNIT_OUTS
    bitswitch *swbus[8];
#endif

    int i;
    for (i = 0; i < 8; i++){

        ledbus[i] = indicator_create("Data");

        char dname[10];
        sprintf(dname,"D%d",i);

#if DISABLE_CTRUNIT_OUTS
        swbus[i] = bitswitch_create(dname);
        bitswitch_setval(swbus[i], 1);

        /// BUS TEST SWITCHES
        bitswitch_connect_out(swbus[i], ledbus[i], (void*)indicator_in_d0);
        bitswitch_connect_out(swbus[i], regA, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regB, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regIN, (void*)reg_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], ram, (void*)ram_8bit_in_dataN[i]);
        bitswitch_connect_out(swbus[i], regout, (void*)reg_out_in_dataN[i]);
#endif

        /// REGA OUTPUT
        reg_8bit_connect_bit_out (regA, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regA, i, regB, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, ram, ram_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regA, i, regout, reg_out_in_dataN[i]);

        /// REGB OUTPUT
        reg_8bit_connect_bit_out (regB, i, ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (regB, i, regA, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, ram, ram_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, regout, reg_out_in_dataN[i]);

        /// REGIN OUTPUT
        if (i < 4){

            reg_8bit_connect_bit_out (regIN, i, ledbus[i], (void*)indicator_in_d0);
            reg_8bit_connect_bit_out (regIN, i, regA, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (regIN, i, regB, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (regIN, i, ram, ram_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (regIN, i, regout, reg_out_in_dataN[i]);
        }

        /// ALU OUTPUT
        alu_8bit_connect_bit_out (alu, i, ledbus[i], (void*)indicator_in_d0);
        alu_8bit_connect_bit_out (alu, i, regA, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regB, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regIN, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, ram, ram_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, regout, reg_out_in_dataN[i]);

        /// RAM OUTPUT
        ram_8bit_connect_bit_out(ram, i, ledbus[i], (void*)indicator_in_d0);
        ram_8bit_connect_bit_out (ram, i, regA, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, regB, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, regIN, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, regout, reg_out_in_dataN[i]);

        int j = 7-i;

        //board_add_led(mainboard,ledbus[i],2+7*j, 4+15, dname, LED_RED);
        board_add_led(bus_board,ledbus[i],1+4*j, 1, dname, LED_RED);

#if DISABLE_CTRUNIT_OUTS
        board_add_manual_switch(mainboard, swbus[i], 2+7*j, 4+18, '0'+i, dname);
#endif
    }

    board_add_board(mainboard,bus_board,1,17);


    for (i = 0; i < 4; i++){

        reg_8bit_connect_bit_out (regA, i, pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (regB, i, pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (regIN, i, pctr, progctr_in_dataN[i]);
        alu_8bit_connect_bit_out (alu, i, pctr, progctr_in_dataN[i]);
        ram_8bit_connect_bit_out (ram, i, pctr, progctr_in_dataN[i]);

#if DISABLE_CTRUNIT_OUTS
        bitswitch_connect_out(swbus[i], pctr, (void*)progctr_in_dataN[i]);
#endif

        progctr_connect_bit_out (pctr, i, ledbus[i], (void*)indicator_in_d0);
        progctr_connect_bit_out (pctr, i, regA, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, regB, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, regIN, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, ram, ram_8bit_in_dataN[i]);
        progctr_connect_bit_out (pctr, i, regout, reg_out_in_dataN[i]);
    }


    //////// CONTROL UNIT //////////////////////////////////////////////////////

    ctrunit *ctru =ctrunit_create("CONTROL UNIT");
    board_object *ctru_board = ctrunit_board_create(ctru, '*', "CONTROL UNIT");
    board_add_board(mainboard,ctru_board,1,32);

    //board_nclock_connect(ctru, (void*)&ctrunit_in_clk);
    clkgen_connect_outn(mainclk, ctru, (void*)&ctrunit_in_clk);

    //// Reset controls
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,ctru,regA);
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,ctru,regB);
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,ctru,regIN);
    ctrunit_connect_out_reset(ctru, ram, (void*)&ram_8bit_in_rst);
    ctrunit_connect_out_nreset(ctru, pctr, (void*)&progctr_in_clear);
    reg_out_in_clear_from((void*)&ctrunit_connect_out_reset,ctru,regout);

    //// Instruction Register Bits -> Control Unit
    reg_8bit_connect_bit_out (regIN, 4, ctru, (void*)&ctrunit_in_instr0);
    reg_8bit_connect_bit_out (regIN, 5, ctru, (void*)&ctrunit_in_instr1);
    reg_8bit_connect_bit_out (regIN, 6, ctru, (void*)&ctrunit_in_instr2);
    reg_8bit_connect_bit_out (regIN, 7, ctru, (void*)&ctrunit_in_instr3);

    //// Controls to the registers
    ctrunit_connect_out_mi(ctru, ram, (void*)&ram_8bit_in_waddr);
    ctrunit_connect_out_ri(ctru, ram, (void*)&ram_8bit_in_wdata);
    ctrunit_connect_out_ro(ctru, ram, (void*)&ram_8bit_in_oe);
    reg_8bit_in_enable_from((void*)&ctrunit_connect_out_io,ctru,regIN);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_ii,ctru,regIN);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_ai,ctru,regA);
    reg_8bit_in_enable_from((void*)&ctrunit_connect_out_ao,ctru,regA);
    alu_8bit_in_enable_from((void*)&ctrunit_connect_out_so, ctru, alu);
    alu_8bit_in_sub_from((void*)&ctrunit_connect_out_su, ctru, alu);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_bi,ctru,regB);
    reg_out_in_load_from((void*)&ctrunit_connect_out_oi,ctru,regout);
    ctrunit_connect_out_ce(ctru, pctr, (void*)&progctr_in_ct_enable);
    ctrunit_connect_out_co(ctru, pctr, (void*)&progctr_in_oenable);
    ctrunit_connect_out_j(ctru, pctr, (void*)&progctr_in_load);

    ////////////////

#if DISABLE_CTRUNIT_OUTS

    bitswitch *sw_ai = bitswitch_create("AI");
    bitswitch *sw_bi = bitswitch_create("BI");
    bitswitch *sw_ii = bitswitch_create("II");

    bitswitch_connect_out(sw_ai, ctru, (void*)&ctrunit_in_ai);
    bitswitch_connect_out(sw_bi, ctru, (void*)&ctrunit_in_bi);
    bitswitch_connect_out(sw_ii, ctru, (void*)&ctrunit_in_ii);

    board_add_manual_switch(mainboard, sw_ai, 42, 2, 'q', "AI");
    board_add_manual_switch(mainboard, sw_bi, 42, 4+6, 'a', "BI");
    board_add_manual_switch(mainboard, sw_ii, 42, 4+10, 'z', "II");

    ////////////////

    bitswitch *sw_ao = bitswitch_create("AO");
    //bitswitch *sw_enable2 = bitswitch_create("ENB");
    bitswitch *sw_io = bitswitch_create("IO");

    bitconst_connect_one(regB,(void*)&reg_8bit_in_enable);
    bitswitch_connect_out(sw_ao, ctru, (void*)&ctrunit_in_ao);
    bitswitch_connect_out(sw_io, ctru, (void*)&ctrunit_in_io);

    board_add_manual_switch(mainboard, sw_ao, 53, 2, 'w', "AO");
    board_add_manual_switch(mainboard, sw_io, 53, 4+10, 'x', "IO");

    ////////////////

    bitswitch *sw_su = bitswitch_create("SU");
    bitswitch *sw_so = bitswitch_create("SO");

    bitswitch_connect_out(sw_su, ctru, (void*)&ctrunit_in_su);
    bitswitch_connect_out(sw_so, ctru, (void*)&ctrunit_in_so);

    board_add_manual_switch(mainboard, sw_su, 42, 6, 'S', "SU");
    board_add_manual_switch(mainboard, sw_so, 53, 6, 'O', "SO");

    ////////////////

    bitswitch *sw_ro = bitswitch_create("RO");
    bitswitch *sw_mi = bitswitch_create("MI");
    bitswitch *sw_ri = bitswitch_create("RI");

    bitswitch_connect_out(sw_ro, ctru, (void*)&ctrunit_in_ro);
    bitswitch_connect_out(sw_mi, ctru, (void*)&ctrunit_in_mi);
    bitswitch_connect_out(sw_ri, ctru, (void*)&ctrunit_in_ri);

    board_add_manual_switch(mainboard, sw_ro, 66, 14, 'j', "RO");
    board_add_manual_switch(mainboard, sw_mi, 74, 14, 'k', "MI");
    board_add_manual_switch(mainboard, sw_ri, 82, 14, 'l', "RI");

    ////////////////

    bitswitch *sw_ce = bitswitch_create("CE");
    bitswitch *sw_co = bitswitch_create("CO");
    bitswitch *sw_j = bitswitch_create("J");

    bitswitch_connect_out(sw_ce, ctru, (void*)&ctrunit_in_ce);
    bitswitch_connect_out(sw_co, ctru, (void*)&ctrunit_in_co);
    bitswitch_connect_out(sw_j, ctru, (void*)&ctrunit_in_j);

    board_add_manual_switch(mainboard, sw_ce, 42, 27, 'b', "CE");
    board_add_manual_switch(mainboard, sw_co, 50, 27, 'n', "CO");
    board_add_manual_switch(mainboard, sw_j, 58, 27, 'm', "J");

    ////////////////

    bitswitch *sw_oi = bitswitch_create("OI");

    bitswitch_connect_out(sw_oi, ctru, (void*)&ctrunit_in_oi);

    board_add_manual_switch(mainboard, sw_oi, 66, 25, 'o', "OI");
#endif

    ////////////////

    board_run(mainboard);

    ////////////////

    DESTROY(regA);
    DESTROY(regB);
    DESTROY(regIN);

    for (i = 0; i < 8; i++){

        DESTROY(ledbus[i]);
#if DISABLE_CTRUNIT_OUTS
        DESTROY(swbus[i]);
#endif
    }

    DESTROY(alu);
    DESTROY(ram);
    DESTROY(pctr);
    DESTROY(regout);

#if DISABLE_CTRUNIT_OUTS
    DESTROY(sw_ai);
    DESTROY(sw_bi);
    DESTROY(sw_ii);
    DESTROY(sw_ao);
    DESTROY(sw_io);
    DESTROY(sw_su);
    DESTROY(sw_so);
    DESTROY(sw_ro);
    DESTROY(sw_mi);
    DESTROY(sw_ri);
    DESTROY(sw_ce);
    DESTROY(sw_co);
    DESTROY(sw_j);
    DESTROY(sw_oi);
#endif

    DESTROY(mainboard);
}
