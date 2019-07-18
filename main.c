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

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){

#if 0
    bitswitch *s = bitswitch_create();
    output *out = output_create("OUT");
    bitswitch_connect_out(s, out, (void*)output_update);
    output_println(out);
    printf("setval 1\n");
    bitswitch_setval(s, 1);
    output_println(out);

#else

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
#endif
    return 0;
}
