////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "computer.h"

////////////////////////////////////////////////////////////////////////////////
void computer_sim(){

    computer_t *comp = malloc(sizeof(computer_t));
    memset(comp, 0, sizeof(computer_t));

    comp->ctx = board_init();

    char name[64];

    sprintf(name,"BEN EATER'S COMPUTER SIM BY ARMCODER V%d.%d.%d",SW_VERSION, SW_REVISION, SW_MINOR);
    comp->mainboard = board_create(0,0,0,name);

    comp->ec = event_init();
    comp->ec->bctx = comp->ctx;

    logger_init(comp->ec);

    comp->mainclk = clkgen_create(comp->ec, "",1000000);

    board_set_clk(comp->ctx, comp->mainclk);

    //////// REGS //////////////////////////////////////////////////////////////

    comp->regA = reg_8bit_create(comp->ec, REG8BIT_NORMAL, "comp->regA");
    comp->regB = reg_8bit_create(comp->ec, REG8BIT_NORMAL, "comp->regB");
    comp->regIN = reg_8bit_create(comp->ec, REG8BIT_IR, "regIN");

    comp->regA_board = reg_8bit_board_create(comp->regA, KEY_F(1), "Reg A"); // Requer NCURSES
    comp->regB_board = reg_8bit_board_create(comp->regB, KEY_F(2), "Reg B"); // Requer NCURSES
    comp->regIN_board = reg_8bit_board_create(comp->regIN, KEY_F(3), "Reg IN"); // Requer NCURSES

    clkgen_connect_out(comp->mainclk, comp->regA, (void*)&reg_8bit_in_clock);
    clkgen_connect_out(comp->mainclk, comp->regB, (void*)&reg_8bit_in_clock);
    clkgen_connect_out(comp->mainclk, comp->regIN, (void*)&reg_8bit_in_clock);

    board_add_board(comp->mainboard,comp->regA_board,1,1);
    board_add_board(comp->mainboard,comp->regB_board,1,9);
    board_add_board(comp->mainboard,comp->regIN_board,1,13);

    //////// ALU ///////////////////////////////////////////////////////////////

    comp->alu = alu_8bit_create(comp->ec, "ALU");
    comp->alu_board = alu_8bit_board_create(comp->alu, KEY_F(4), "ALU"); // Requer NCURSES
    board_add_board(comp->mainboard,comp->alu_board,1,5);

    ls173_connect_1q(comp->regA->ls173_lo, comp->alu, alu_8bit_in_dataAN[0]);
    ls173_connect_2q(comp->regA->ls173_lo, comp->alu, alu_8bit_in_dataAN[1]);
    ls173_connect_3q(comp->regA->ls173_lo, comp->alu, alu_8bit_in_dataAN[2]);
    ls173_connect_4q(comp->regA->ls173_lo, comp->alu, alu_8bit_in_dataAN[3]);
    ls173_connect_1q(comp->regA->ls173_hi, comp->alu, alu_8bit_in_dataAN[4]);
    ls173_connect_2q(comp->regA->ls173_hi, comp->alu, alu_8bit_in_dataAN[5]);
    ls173_connect_3q(comp->regA->ls173_hi, comp->alu, alu_8bit_in_dataAN[6]);
    ls173_connect_4q(comp->regA->ls173_hi, comp->alu, alu_8bit_in_dataAN[7]);

    ls173_connect_1q(comp->regB->ls173_lo, comp->alu, alu_8bit_in_dataBN[0]);
    ls173_connect_2q(comp->regB->ls173_lo, comp->alu, alu_8bit_in_dataBN[1]);
    ls173_connect_3q(comp->regB->ls173_lo, comp->alu, alu_8bit_in_dataBN[2]);
    ls173_connect_4q(comp->regB->ls173_lo, comp->alu, alu_8bit_in_dataBN[3]);
    ls173_connect_1q(comp->regB->ls173_hi, comp->alu, alu_8bit_in_dataBN[4]);
    ls173_connect_2q(comp->regB->ls173_hi, comp->alu, alu_8bit_in_dataBN[5]);
    ls173_connect_3q(comp->regB->ls173_hi, comp->alu, alu_8bit_in_dataBN[6]);
    ls173_connect_4q(comp->regB->ls173_hi, comp->alu, alu_8bit_in_dataBN[7]);

    //////// RAM ///////////////////////////////////////////////////////////////

    comp->ram = ram_8bit_create(comp->ec, "RAM");
    comp->ram_board = ram_8bit_board_create(comp->ram, KEY_F(5), "RAM"); // Requer NCURSES
    board_add_board(comp->mainboard,comp->ram_board,42,1);
    clkgen_connect_out(comp->mainclk, comp->ram, (void*)&ram_8bit_in_clk);

    //////// PROGRAM COUNTER ///////////////////////////////////////////////////

    comp->pctr = progctr_create(comp->ec, "PC");
    comp->pctr_board = progctr_board_create(comp->pctr, KEY_F(6), "PC");
    board_add_board(comp->mainboard,comp->pctr_board,67,21);
    clkgen_connect_out(comp->mainclk, comp->pctr, (void*)&progctr_in_clock);

    //////// REG OUT ///////////////////////////////////////////////////////////

    comp->regout = reg_out_create(comp->ec, "RO");
    comp->regout_board = reg_out_board_create(comp->regout, KEY_F(6), "RO");
    board_add_board(comp->mainboard,comp->regout_board,60,14);
    clkgen_connect_out(comp->mainclk, comp->regout, (void*)&reg_out_in_clock);

    //////// BUS ///////////////////////////////////////////////////////////////

    comp->bus_board = board_create(40, 4, 0, "BUS");

    int i;
    for (i = 0; i < 8; i++){

        comp->ledbus[i] = indicator_create(comp->ec, "Data");

        char dname[10];
        sprintf(dname,"D%d",i);

        /// BUS PULL DOWNS
        pulldown_connect(comp->ledbus[i], (void*)indicator_in_d0);
        pulldown_connect(comp->regA, reg_8bit_in_dataN[i]);
        pulldown_connect(comp->regB, reg_8bit_in_dataN[i]);
        pulldown_connect(comp->regIN, reg_8bit_in_dataN[i]);
        pulldown_connect(comp->ram, ram_8bit_in_dataN[i]);
        pulldown_connect(comp->regout, reg_out_in_dataN[i]);

        /// comp->regA OUTPUT
		reg_8bit_connect_bit_out (comp->regA, i, comp->ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (comp->regA, i, comp->regB, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regA, i, comp->regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regA, i, comp->ram, ram_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regA, i, comp->regout, reg_out_in_dataN[i]);

        /// comp->regB OUTPUT
        reg_8bit_connect_bit_out (comp->regB, i, comp->ledbus[i], (void*)indicator_in_d0);
        reg_8bit_connect_bit_out (comp->regB, i, comp->regA, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regB, i, comp->regIN, reg_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regB, i, comp->ram, ram_8bit_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regB, i, comp->regout, reg_out_in_dataN[i]);

        /// REGIN OUTPUT
        if (i < 4){

            reg_8bit_connect_bit_out (comp->regIN, i, comp->ledbus[i], (void*)indicator_in_d0);
            reg_8bit_connect_bit_out (comp->regIN, i, comp->regA, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (comp->regIN, i, comp->regB, reg_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (comp->regIN, i, comp->ram, ram_8bit_in_dataN[i]);
            reg_8bit_connect_bit_out (comp->regIN, i, comp->regout, reg_out_in_dataN[i]);
        }

        /// ALU OUTPUT
        alu_8bit_connect_bit_out (comp->alu, i, comp->ledbus[i], (void*)indicator_in_d0);
        alu_8bit_connect_bit_out (comp->alu, i, comp->regA, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (comp->alu, i, comp->regB, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (comp->alu, i, comp->regIN, reg_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (comp->alu, i, comp->ram, ram_8bit_in_dataN[i]);
        alu_8bit_connect_bit_out (comp->alu, i, comp->regout, reg_out_in_dataN[i]);

        /// RAM OUTPUT
        ram_8bit_connect_bit_out(comp->ram, i, comp->ledbus[i], (void*)indicator_in_d0);
        ram_8bit_connect_bit_out (comp->ram, i, comp->regA, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (comp->ram, i, comp->regB, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (comp->ram, i, comp->regIN, reg_8bit_in_dataN[i]);
        ram_8bit_connect_bit_out (comp->ram, i, comp->regout, reg_out_in_dataN[i]);

        int j = 7-i;

        board_add_led(comp->bus_board,comp->ledbus[i],1+4*j, 1, dname, LED_RED);
    }

    board_add_board(comp->mainboard,comp->bus_board,1,17);

    for (i = 0; i < 4; i++){

        reg_8bit_connect_bit_out (comp->regA, i, comp->pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regB, i, comp->pctr, progctr_in_dataN[i]);
        reg_8bit_connect_bit_out (comp->regIN, i, comp->pctr, progctr_in_dataN[i]);
        alu_8bit_connect_bit_out (comp->alu, i, comp->pctr, progctr_in_dataN[i]);
        ram_8bit_connect_bit_out (comp->ram, i, comp->pctr, progctr_in_dataN[i]);

        progctr_connect_bit_out (comp->pctr, i, comp->ledbus[i], (void*)indicator_in_d0);
        progctr_connect_bit_out (comp->pctr, i, comp->regA, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (comp->pctr, i, comp->regB, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (comp->pctr, i, comp->regIN, reg_8bit_in_dataN[i]);
        progctr_connect_bit_out (comp->pctr, i, comp->ram, ram_8bit_in_dataN[i]);
        progctr_connect_bit_out (comp->pctr, i, comp->regout, reg_out_in_dataN[i]);
    }

    //////// CONTROL UNIT //////////////////////////////////////////////////////

    comp->ctru = ctrunit_create(comp->ec, "CONTROL UNIT");
    comp->ctru_board = ctrunit_board_create(comp->ctru, '*', "CONTROL UNIT");
    board_add_board(comp->mainboard,comp->ctru_board,1,21);

    comp->ctru_flags_board = ctrunit_board_flags_create(comp->ctru, '*', "FLAGS");
    board_add_board(comp->mainboard,comp->ctru_flags_board,44,15);

    clkgen_connect_out(comp->mainclk, comp->ctru, (void*)&ctrunit_in_clk);
    clkgen_connect_outn(comp->mainclk, comp->ctru, (void*)&ctrunit_in_clkn);

    alu_8bit_connect_zero_out (comp->alu, comp->ctru, (void*)&ctrunit_in_zero);
    alu_8bit_connect_carry_out (comp->alu, comp->ctru, (void*)&ctrunit_in_carry);

    //// Reset controls
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,comp->ctru,comp->regA);
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,comp->ctru,comp->regB);
    reg_8bit_in_clear_from((void*)&ctrunit_connect_out_reset,comp->ctru,comp->regIN);
    ctrunit_connect_out_reset(comp->ctru, comp->ram, (void*)&ram_8bit_in_rst);
    ctrunit_connect_out_nreset(comp->ctru, comp->pctr, (void*)&progctr_in_clear);
    reg_out_in_clear_from((void*)&ctrunit_connect_out_reset,comp->ctru,comp->regout);

    //// Instruction Register Bits -> Control Unit
    reg_8bit_connect_bit_out (comp->regIN, 4, comp->ctru, (void*)&ctrunit_in_instr0);
    reg_8bit_connect_bit_out (comp->regIN, 5, comp->ctru, (void*)&ctrunit_in_instr1);
    reg_8bit_connect_bit_out (comp->regIN, 6, comp->ctru, (void*)&ctrunit_in_instr2);
    reg_8bit_connect_bit_out (comp->regIN, 7, comp->ctru, (void*)&ctrunit_in_instr3);

    //// Controls to the registers
    ctrunit_connect_out_hlt(comp->ctru, comp->mainclk, (void*)&clkgen_in_halt);
    ctrunit_connect_out_mi(comp->ctru, comp->ram, (void*)&ram_8bit_in_waddr);
    ctrunit_connect_out_ri(comp->ctru, comp->ram, (void*)&ram_8bit_in_wdata);
    ctrunit_connect_out_ro(comp->ctru, comp->ram, (void*)&ram_8bit_in_oe);
    reg_8bit_in_enable_from((void*)&ctrunit_connect_out_io,comp->ctru,comp->regIN);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_ii,comp->ctru,comp->regIN);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_ai,comp->ctru,comp->regA);
    reg_8bit_in_enable_from((void*)&ctrunit_connect_out_ao,comp->ctru,comp->regA);
    alu_8bit_in_enable_from((void*)&ctrunit_connect_out_so, comp->ctru, comp->alu);
    alu_8bit_in_sub_from((void*)&ctrunit_connect_out_su, comp->ctru, comp->alu);
    reg_8bit_in_load_from((void*)&ctrunit_connect_out_bi,comp->ctru,comp->regB);
    reg_out_in_load_from((void*)&ctrunit_connect_out_oi,comp->ctru,comp->regout);
    ctrunit_connect_out_ce(comp->ctru, comp->pctr, (void*)&progctr_in_ct_enable);
    ctrunit_connect_out_co(comp->ctru, comp->pctr, (void*)&progctr_in_oenable);
    ctrunit_connect_out_j(comp->ctru, comp->pctr, (void*)&progctr_in_load);

    ////////////////

#include "exmachina.h"

    pthread_t sim_thread;
    pthread_create(&sim_thread, NULL, exmachina_thread, comp->ctx);
    pthread_detach(sim_thread);

    board_run(comp->ctx, comp->ec, comp->mainboard);

    logger_end(comp->ec);

    DESTROY(comp->mainclk);

    ////////////////

    DESTROY(comp->regA);
    DESTROY(comp->regB);
    DESTROY(comp->regIN);

    for (i = 0; i < 8; i++){

        DESTROY(comp->ledbus[i]);
    }

    DESTROY(comp->alu);
    DESTROY(comp->ram);
    DESTROY(comp->pctr);
    DESTROY(comp->regout);

    DESTROY(comp->mainboard);

    free(comp);
}
