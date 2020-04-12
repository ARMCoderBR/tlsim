/*
 * testls191.c
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#include <stdlib.h>
#include <ncurses.h>

#include "update.h"
#include "ls191.h"
#include "indicator.h"
#include "bitswitch.h"
#include "board.h"

typedef struct {

    ls191 *ctr1;
    indicator *oqa1;
    indicator *oqb1;
    indicator *oqc1;
    indicator *oqd1;
    indicator *ohex1;

    indicator *omaxmin1;
    indicator *oripclk1;
    indicator *oclk;
    indicator *oupdown;

    bitswitch *clk;
    bitswitch *updownsel;

    bitswitch *level0;
    bitswitch *level1;

} testls191;


////////////////////////////////////////////////////////////////////////////////
testls191 *testls191_create(){

    testls191 *b = malloc(sizeof(testls191));
    if (!b) return b;

    b->ctr1 = ls191_create();
    b->oqa1 = indicator_create(NULL);
    b->oqb1 = indicator_create(NULL);
    b->oqc1 = indicator_create(NULL);
    b->oqd1 = indicator_create(NULL);
    b->ohex1 = indicator_create("Hex");

    b->omaxmin1 = indicator_create("MAXMIN");
    b->oripclk1 = indicator_create("RIPCLK");
    b->oclk = indicator_create("CLK");
    b->oupdown = indicator_create("-UP/+DOWN");

    b->clk = bitswitch_create();
    b->updownsel = bitswitch_create();

    b->level0 = bitswitch_create();
    b->level1 = bitswitch_create();

    bitswitch_setval(b->level0,0);
    bitswitch_setval(b->level1,1);

    bitswitch_connect_out(b->level1,b->ctr1,(void*)&ls191_in_load);

    bitswitch_connect_out(b->level0,b->ctr1,(void*)&ls191_in_enable);

    bitswitch_connect_out(b->updownsel,b->ctr1,(void*)&ls191_in_updown);
    bitswitch_connect_out(b->updownsel, b->oupdown, (void*)&indicator_in_d0);

    bitswitch_connect_out(b->clk, b->ctr1, (void*)&ls191_in_clk);

    bitswitch_connect_out(b->clk, b->oclk, (void*)&indicator_in_d0);

    ls191_connect_qa(b->ctr1, b->oqa1, (void*)&indicator_in_d0);
    ls191_connect_qb(b->ctr1, b->oqb1, (void*)&indicator_in_d0);
    ls191_connect_qc(b->ctr1, b->oqc1, (void*)&indicator_in_d0);
    ls191_connect_qd(b->ctr1, b->oqd1, (void*)&indicator_in_d0);

    ls191_connect_qa(b->ctr1, b->ohex1, (void*)&indicator_in_d0);
    ls191_connect_qb(b->ctr1, b->ohex1, (void*)&indicator_in_d1);
    ls191_connect_qc(b->ctr1, b->ohex1, (void*)&indicator_in_d2);
    ls191_connect_qd(b->ctr1, b->ohex1, (void*)&indicator_in_d3);

    ls191_connect_maxmin(b->ctr1, b->omaxmin1, (void*)&indicator_in_d0);
    ls191_connect_ripclk(b->ctr1, b->oripclk1, (void*)&indicator_in_d0);

    return b;
}

////////////////////////////////////////////////////////////////////////////////
board_object *testls191_board_create(testls191 *t, int key, char *name){

    board_object *board = board_create(40,7, key, name);

    if (!board) return board;

    board_add_led(board, t->oclk,1,1,"CLK");
    board_add_xdigit(board, t->ohex1,10,1,"COUNT");
    board_add_led(board, t->omaxmin1,20,1,"MAXMIN");
    board_add_led(board, t->oripclk1,30,1,"RIPPLE");
    board_add_manual_switch(board, t->updownsel, 1, 4, KEY_F(1), "UP/DN");

    return board;
}

////////////////////////////////////////////////////////////////////////////////
void do_testls191(){

    testls191 *t191_1 = testls191_create();

    if (!t191_1){

        perror("t191_1 create");
        exit(0);
    }

    printf("### UP\n");
    bitswitch_setval(t191_1->updownsel,0);  //UP


    board_object *b191_b1 = testls191_board_create(t191_1, 0, "LS191 TEST");

    if (!b191_b1){

        perror("ls191_board1 create");
        exit(0);
    }

    board_assign_clock_to_switch(t191_1->clk);

    board_object *mainboard = mainboard_create("My LS191 Test");
    board_add_board(mainboard, b191_b1, 8, 13);
    board_run(mainboard);

    //board_run(ls191_test1);
}
