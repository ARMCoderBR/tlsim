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

////////////////////////////////////////////////////////////////////////////////
void testls191(){

    ls191 *ctr1 = ls191_create();
    indicator *oqa1 = indicator_create(NULL);
    indicator *oqb1 = indicator_create(NULL);
    indicator *oqc1 = indicator_create(NULL);
    indicator *oqd1 = indicator_create(NULL);
    indicator *ohex1 = indicator_create("Hex");

    indicator *omaxmin1 = indicator_create("MAXMIN");
    indicator *oripclk1 = indicator_create("RIPCLK");
    indicator *oclk = indicator_create("CLK");
    indicator *oupdown = indicator_create("-UP/+DOWN");

    bitswitch *clk = bitswitch_create();
    bitswitch *updownsel = bitswitch_create();

    bitswitch *level0 = bitswitch_create();
    bitswitch *level1 = bitswitch_create();
    bitswitch_setval(level0,0);
    bitswitch_setval(level1,1);

    bitswitch_connect_out(level1,ctr1,(void*)&ls191_in_load);

    bitswitch_connect_out(level0,ctr1,(void*)&ls191_in_enable);

    bitswitch_connect_out(updownsel,ctr1,(void*)&ls191_in_updown);
    bitswitch_connect_out(updownsel, oupdown, (void*)&indicator_in_d0);

    bitswitch_connect_out(clk, ctr1, (void*)&ls191_in_clk);

    bitswitch_connect_out(clk, oclk, (void*)&indicator_in_d0);

    ls191_connect_qa(ctr1, oqa1, (void*)&indicator_in_d0);
    ls191_connect_qb(ctr1, oqb1, (void*)&indicator_in_d0);
    ls191_connect_qc(ctr1, oqc1, (void*)&indicator_in_d0);
    ls191_connect_qd(ctr1, oqd1, (void*)&indicator_in_d0);

    ls191_connect_qa(ctr1, ohex1, (void*)&indicator_in_d0);
    ls191_connect_qb(ctr1, ohex1, (void*)&indicator_in_d1);
    ls191_connect_qc(ctr1, ohex1, (void*)&indicator_in_d2);
    ls191_connect_qd(ctr1, ohex1, (void*)&indicator_in_d3);

    ls191_connect_maxmin(ctr1, omaxmin1, (void*)&indicator_in_d0);
    ls191_connect_ripclk(ctr1, oripclk1, (void*)&indicator_in_d0);

    printf("### UP\n");
    bitswitch_setval(updownsel,0);  //UP

#if 0
    int i;

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        indicator_prints(oclk); indicator_prints(oupdown); indicator_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); indicator_prints(oqa1); indicator_prints(ohex1); indicator_println(omaxmin1);
        bitswitch_setval(clk,1);
        indicator_prints(oclk); indicator_prints(oupdown); indicator_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); indicator_prints(oqa1); indicator_prints(ohex1); indicator_println(omaxmin1);
    }

    printf("### DOWN\n");
    bitswitch_setval(updownsel,1);  //DOWN
    indicator_prints(oclk); indicator_prints(oupdown); indicator_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); indicator_prints(oqa1); indicator_prints(ohex1); indicator_println(omaxmin1);

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        indicator_prints(oclk); indicator_prints(oupdown); indicator_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); indicator_prints(oqa1); indicator_prints(ohex1); indicator_println(omaxmin1);
        bitswitch_setval(clk,1);
        indicator_prints(oclk); indicator_prints(oupdown); indicator_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); indicator_prints(oqa1); indicator_prints(ohex1); indicator_println(omaxmin1);
    }
#endif

    board_object *mainboard = board_create(0,0,0,"LS191 TEST");

    if (!mainboard){

        perror("mainboard create");
        exit(0);
    }


    board_assign_clock_to_switch(clk);

    board_add_led(mainboard, oclk,1,1,"CLK");

    board_add_xdigit(mainboard, ohex1,10,1,"COUNT");

    board_add_led(mainboard, omaxmin1,20,1,"MAXMIN");

    board_add_led(mainboard, oripclk1,30,1,"RIPPLE");

    board_add_manual_switch(mainboard, updownsel, 1, 4, KEY_F(1), "UP/DN");

    board_run(mainboard);
}
