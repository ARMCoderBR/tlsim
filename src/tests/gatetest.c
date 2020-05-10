////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "update.h"
#include "bitswitch.h"
#include "ls08.h"
#include "indicator.h"

////////////////////////////////////////////////////////////////////////////////
void gatetest(){

    bitswitch *s1,*s2,*s3;
    ls08 *ands;
    indicator *os1,*os2,*os3,*oand1,*oand2;

    s1 = bitswitch_create("S1");
    s2 = bitswitch_create("S2");
    s3 = bitswitch_create("S3");
    ands = ls08_create();
    os1 = indicator_create("S1");
    os2 = indicator_create("S2");
    os3 = indicator_create("S3");
    oand1 = indicator_create("AND1");
    oand2 = indicator_create("AND2");

    bitswitch_connect_out(s1, ands, (void*)&ls08_in_a1);
    bitswitch_connect_out(s2, ands, (void*)&ls08_in_b1);
    bitswitch_connect_out(s3, ands, (void*)&ls08_in_a2);

    ls08_connect_y1(ands, ands, (void*)&ls08_in_b2);
    ls08_connect_y1(ands, oand1, (void*)&indicator_in_d0);

    bitswitch_connect_out(s1, os1, (void*)&indicator_in_d0);
    bitswitch_connect_out(s2, os2, (void*)&indicator_in_d0);
    bitswitch_connect_out(s3, os3, (void*)&indicator_in_d0);
    ls08_connect_y2(ands, oand2, (void*)&indicator_in_d0);

    printf("===================== 0 ==== 0\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 0);

    indicator_prints(os1);
    indicator_prints(os2);
    indicator_prints(os3);
    indicator_prints(oand1);
    indicator_println(oand2);

    printf("===================== 1 ==== 0\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 0);

    indicator_prints(os1);
    indicator_prints(os2);
    indicator_prints(os3);
    indicator_prints(oand1);
    indicator_println(oand2);

    printf("===================== 0 ==== 1\n");

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 1);

    indicator_prints(os1);
    indicator_prints(os2);
    indicator_prints(os3);
    indicator_prints(oand1);
    indicator_println(oand2);

    printf("===================== 1 ==== 1\n");

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 1);

    indicator_prints(os1);
    indicator_prints(os2);
    indicator_prints(os3);
    indicator_prints(oand1);
    indicator_println(oand2);

    printf("===================== 1 ==== 1 and 1\n");

    bitswitch_setval(s3, 1);

    indicator_prints(os1);
    indicator_prints(os2);
    indicator_prints(os3);
    indicator_prints(oand1);
    indicator_println(oand2);
}
