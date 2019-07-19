/*
 * main.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include <stdio.h>

#include "bitand.h"
#include "bitswitch.h"
#include "output.h"
#include "update.h"
#include "ls191.h"

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){

    bitswitch *s1,*s2,*s3;
    bitand *and1,*and2;
    output *os1,*os2,*os3,*oand1,*oand2;

    s1 = bitswitch_create();
    s2 = bitswitch_create();
    s3 = bitswitch_create();
    and1 = bitand_create();
    and2 = bitand_create();
    os1 = output_create("S1");
    os2 = output_create("S2");
    os3 = output_create("S3");
    oand1 = output_create("AND1");
    oand2 = output_create("AND2");

    bitswitch_connect_out(s1, and1, (void*)&bitand_update_ina);
    bitswitch_connect_out(s2, and1, (void*)&bitand_update_inb);
    bitswitch_connect_out(s3, and2, (void*)&bitand_update_ina);
    bitand_connect_out(and1, and2, (void*)&bitand_update_inb);
    bitand_connect_out(and1, oand1, (void*)&output_update);

    bitswitch_connect_out(s1, os1, (void*)&output_update);
    bitswitch_connect_out(s2, os2, (void*)&output_update);
    bitand_connect_out(and2, oand2, (void*)&output_update);

    printf("===================== 0 ==== 0\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 0);

    output_print(os1);
    output_print(os2);
    output_print(os3);
    output_print(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 0\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 0);

    output_print(os1);
    output_print(os2);
    output_print(os3);
    output_print(oand1);
    output_println(oand2);

    printf("===================== 0 ==== 1\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 1);

    output_print(os1);
    output_print(os2);
    output_print(os3);
    output_print(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 1\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 1);

    output_print(os1);
    output_print(os2);
    output_print(os3);
    output_print(oand1);
    output_println(oand2);

    printf("===================== 1 ==== 1 and 1\n");

    bitswitch_setval(s3, 1);

    output_print(os1);
    output_print(os2);
    output_print(os3);
    output_print(oand1);
    output_println(oand2);

    ////////////////////////////////////////////////////////////////////////////

    ls191 *ctr1 = ls191_create();
    output *oqa1 = output_create("QA");
    output *oqb1 = output_create("QB");
    output *oqc1 = output_create("QC");
    output *oqd1 = output_create("QD");
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
    bitswitch_connect_out(updownsel, oupdown, (void*)&output_update);

    bitswitch_connect_out(clk, ctr1, (void*)&ls191_update_clk);

    bitswitch_connect_out(clk, oclk, (void*)&output_update);

    ls191_connect_qa(ctr1, oqa1, (void*)&output_update);
    ls191_connect_qb(ctr1, oqb1, (void*)&output_update);
    ls191_connect_qc(ctr1, oqc1, (void*)&output_update);
    ls191_connect_qd(ctr1, oqd1, (void*)&output_update);
    ls191_connect_maxmin(ctr1, omaxmin1, (void*)&output_update);
    ls191_connect_ripclk(ctr1, oripclk1, (void*)&output_update);

    int i;

    printf("### UP\n");
    bitswitch_setval(updownsel,0);  //UP

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        output_print(oclk); output_print(oupdown); output_print(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_print(oqa1); output_println(omaxmin1);
        bitswitch_setval(clk,1);
        output_print(oclk); output_print(oupdown); output_print(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_print(oqa1); output_println(omaxmin1);
    }

    printf("### DOWN\n");
    bitswitch_setval(updownsel,1);  //DOWN
    output_print(oclk); output_print(oupdown); output_print(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_print(oqa1); output_println(omaxmin1);

    for (i = 0; i < 16; i++){
        bitswitch_setval(clk,0);
        output_print(oclk); output_print(oupdown); output_print(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_print(oqa1); output_println(omaxmin1);
        bitswitch_setval(clk,1);
        output_print(oclk); output_print(oupdown); output_print(oripclk1); output_print(oqd1); output_print(oqc1); output_print(oqb1); output_print(oqa1); output_println(omaxmin1);
    }

    return 0;
}
