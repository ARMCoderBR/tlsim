/*
 * main.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include <stdio.h>

#include "bitswitch.h"
#include "ls08.h"
#include "output.h"
#include "update.h"
#include "ls191.h"

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){

    bitswitch *s1,*s2,*s3;
    ls08 *ands;
    output *os1,*os2,*os3,*oand1,*oand2;

    s1 = bitswitch_create();
    s2 = bitswitch_create();
    s3 = bitswitch_create();
    ands = ls08_create();
    os1 = output_create("S1");
    os2 = output_create("S2");
    os3 = output_create("S3");
    oand1 = output_create("AND1");
    oand2 = output_create("AND2");

    bitswitch_connect_out(s1, ands, (void*)&ls08_update_a1);
    bitswitch_connect_out(s2, ands, (void*)&ls08_update_b1);
    bitswitch_connect_out(s3, ands, (void*)&ls08_update_a2);

    ls08_connect_y1(ands, ands, (void*)&ls08_update_b2);
    ls08_connect_y1(ands, oand1, (void*)&output_update_d0);

    bitswitch_connect_out(s1, os1, (void*)&output_update_d0);
    bitswitch_connect_out(s2, os2, (void*)&output_update_d0);
    bitswitch_connect_out(s3, os3, (void*)&output_update_d0);
    ls08_connect_y2(ands, oand2, (void*)&output_update_d0);

    printf("===================== 0 ==== 0\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 0);

    output_prints(os1);
    output_prints(os2);
    output_prints(os3);
    output_prints(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 0\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 0);

    output_prints(os1);
    output_prints(os2);
    output_prints(os3);
    output_prints(oand1);
    output_println(oand2);

    printf("===================== 0 ==== 1\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 1);

    output_prints(os1);
    output_prints(os2);
    output_prints(os3);
    output_prints(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 1\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 1);

    output_prints(os1);
    output_prints(os2);
    output_prints(os3);
    output_prints(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 1 and 1\n");

    bitswitch_setval(s3, 1);

    output_prints(os1);
    output_prints(os2);
    output_prints(os3);
    output_prints(oand1);
    output_println(oand2);

    ////////////////////////////////////////////////////////////////////////////

    ls191 *ctr1 = ls191_create();
    output *oqa1 = output_create(NULL);
    output *oqb1 = output_create(NULL);
    output *oqc1 = output_create(NULL);
    output *oqd1 = output_create(NULL);
    output *ohex1 = output_create("Hex");

    output *omaxmin1 = output_create("MAXMIN");
    output *oripclk1 = output_create("RIPCLK");
    output *oclk = output_create("CLK");
    output *oupdown = output_create("-UP/+DOWN");

    bitswitch *clk = bitswitch_create();
    bitswitch *updownsel = bitswitch_create();

    bitswitch *level0 = bitswitch_create();
    bitswitch *level1 = bitswitch_create();
    bitswitch_setval(level0,0);
    bitswitch_setval(level1,1);

    bitswitch_connect_out(level1,ctr1,(void*)&ls191_update_load);

    bitswitch_connect_out(level0,ctr1,(void*)&ls191_update_enable);

    bitswitch_connect_out(updownsel,ctr1,(void*)&ls191_update_downup);
    bitswitch_connect_out(updownsel, oupdown, (void*)&output_update_d0);

    bitswitch_connect_out(clk, ctr1, (void*)&ls191_update_clk);

    bitswitch_connect_out(clk, oclk, (void*)&output_update_d0);

    ls191_connect_qa(ctr1, oqa1, (void*)&output_update_d0);
    ls191_connect_qb(ctr1, oqb1, (void*)&output_update_d0);
    ls191_connect_qc(ctr1, oqc1, (void*)&output_update_d0);
    ls191_connect_qd(ctr1, oqd1, (void*)&output_update_d0);

    ls191_connect_qa(ctr1, ohex1, (void*)&output_update_d0);
    ls191_connect_qb(ctr1, ohex1, (void*)&output_update_d1);
    ls191_connect_qc(ctr1, ohex1, (void*)&output_update_d2);
    ls191_connect_qd(ctr1, ohex1, (void*)&output_update_d3);

    ls191_connect_maxmin(ctr1, omaxmin1, (void*)&output_update_d0);
    ls191_connect_ripclk(ctr1, oripclk1, (void*)&output_update_d0);

    int i;

    printf("### UP\n");
    bitswitch_setval(updownsel,0);  //UP

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        output_prints(oclk); output_prints(oupdown); output_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_prints(oqa1); output_prints(ohex1); output_println(omaxmin1);
        bitswitch_setval(clk,1);
        output_prints(oclk); output_prints(oupdown); output_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_prints(oqa1); output_prints(ohex1); output_println(omaxmin1);
    }

    printf("### DOWN\n");
    bitswitch_setval(updownsel,1);  //DOWN
    output_prints(oclk); output_prints(oupdown); output_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_prints(oqa1); output_prints(ohex1); output_println(omaxmin1);

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        output_prints(oclk); output_prints(oupdown); output_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_prints(oqa1); output_prints(ohex1); output_println(omaxmin1);
        bitswitch_setval(clk,1);
        output_prints(oclk); output_prints(oupdown); output_prints(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_prints(oqa1); output_prints(ohex1); output_println(omaxmin1);
    }

    return 0;
}
