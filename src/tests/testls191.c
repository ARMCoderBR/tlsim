////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <ncurses.h>

#include "update.h"
#include "ls191.h"
#include "indicator.h"
#include "bitswitch.h"
#include "bitconst.h"
#include "board.h"
#include "ls32.h"

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

    board_object *board = board_create(40,4, key, name);
    if (!board) return board;

    board_add_led(board, t->oclk,1,1,"CLK", LED_RED);
    board_add_xdigit(board, t->ohex1,10,1,"COUNT", LED_RED);
    board_add_led(board, t->omaxmin1,20,1,"MAXMIN", LED_RED);
    board_add_led(board, t->oripclk1,30,1,"RIPPLE", LED_RED);

    return board;
}

////////////////////////////////////////////////////////////////////////////////
void do_testls191(){

    testls191 *t191_1 = testls191_create();
    testls191 *t191_2 = testls191_create();
    testls191 *t191_3 = testls191_create();
    testls191 *t191_4 = testls191_create();

    if ((!t191_1)||(!t191_2)||(!t191_3)||(!t191_4))    {

        perror("t191_x create");
        exit(0);
    }


    bitswitch *updownsel = bitswitch_create("UP/DN");
    if (!updownsel){

        perror("updownsel create");
        exit(0);
    }

    bitswitch_setval(updownsel, 0);  //UP

    bitswitch_connect_out(updownsel,t191_1->ctr1,(void*)&ls191_in_updown);
    bitswitch_connect_out(updownsel,t191_2->ctr1,(void*)&ls191_in_updown);
    bitswitch_connect_out(updownsel,t191_3->ctr1,(void*)&ls191_in_updown);
    bitswitch_connect_out(updownsel,t191_4->ctr1,(void*)&ls191_in_updown);


    board_object *b191_b1 = testls191_board_create(t191_1, 0, "LS191-1");
    board_object *b191_b2 = testls191_board_create(t191_2, 0, "LS191-2");
    board_object *b191_b3 = testls191_board_create(t191_3, 0, "LS191-3");
    board_object *b191_b4 = testls191_board_create(t191_4, 0, "LS191-4");

    if ((!b191_b1)||(!b191_b2)||(!b191_b3)||(!b191_b4)){

        perror("b191_bx create");
        exit(0);
    }


    ls32 *quad_or1 = ls32_create();
    if (!quad_or1){

        perror("quad_or1 create");
        exit(0);
    }

/*

SYSCLK
--+---------------+---------------+---------------+
  |CLK            |CLK            |CLK            |CLK
+-------+ EN    +-------+ EN    +-------+ EN    +-------+ EN
|LS191-1|--+    |LS191-2|--+    |LS191-3|--+    |LS191-4|----+
+-------+  |    +-------+  |    +-------+  |    +-------+    vGND
           |          |RIP |          |RIP |          |RIP
           |    /-/B2-+    |    /-/B1-+    |          |
           +-Y2( (         +-Y1( (         |          |
                \-\A2------+    \-\A1------+----------+
               LS32(2)         LS32(1)
*/

    board_object *mainboard = mainboard_create("My LS191 Test");
    board_add_board(mainboard, b191_b1, 2, 4);
    board_add_board(mainboard, b191_b2, 42, 4);

    board_add_board(mainboard, b191_b3, 2, 10);
    board_add_board(mainboard, b191_b4, 42, 10);


    board_add_manual_switch(mainboard, updownsel, 2, 15, KEY_F(1), "UP/DN");



    board_clock_connect(t191_1->ctr1, (void*)&ls191_in_clk);
    board_clock_connect(t191_1->oclk, (void*)&indicator_in_d0);
    bitconst_connect_one(t191_1->ctr1,(void*)&ls191_in_load);
    //bitconst_connect_zero(t191_1->ctr1,(void*)&ls191_in_enable);


    board_clock_connect(t191_2->ctr1, (void*)&ls191_in_clk);
    board_clock_connect(t191_2->oclk, (void*)&indicator_in_d0);
    bitconst_connect_one(t191_2->ctr1,(void*)&ls191_in_load);
    //bitconst_connect_zero(t191_2->ctr1,(void*)&ls191_in_enable);
    ls191_connect_ripclk(t191_2->ctr1, quad_or1,(void*)&ls32_in_b2);
    ls32_connect_y2(quad_or1,t191_1->ctr1,(void*)&ls191_in_enable);


    board_clock_connect(t191_3->ctr1, (void*)&ls191_in_clk);
    board_clock_connect(t191_3->oclk, (void*)&indicator_in_d0);
    bitconst_connect_one(t191_3->ctr1,(void*)&ls191_in_load);
    //bitconst_connect_zero(t191_3->ctr1,(void*)&ls191_in_enable);
    ls191_connect_ripclk(t191_3->ctr1, quad_or1, (void*)&ls32_in_b1);
    ls32_connect_y1(quad_or1,quad_or1,(void*)&ls32_in_a2);
    ls32_connect_y1(quad_or1,t191_2->ctr1, (void*)&ls191_in_enable);


    board_clock_connect(t191_4->ctr1, (void*)&ls191_in_clk);
    board_clock_connect(t191_4->oclk, (void*)&indicator_in_d0);
    bitconst_connect_one(t191_4->ctr1,(void*)&ls191_in_load);
    bitconst_connect_zero(t191_4->ctr1,(void*)&ls191_in_enable);
    ls191_connect_ripclk(t191_4->ctr1, t191_3->ctr1, (void*)&ls191_in_enable);
    ls191_connect_ripclk(t191_4->ctr1, quad_or1, (void*)&ls32_in_a1);

    board_run(mainboard);
}
