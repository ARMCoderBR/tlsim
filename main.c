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

    bitswitch *s1,*s2;
    bitand *and1;
    output *os1,*os2,*oand;

    s1 = bitswitch_create();
    s2 = bitswitch_create();
    and1 = bitand_create();
    os1 = output_create("S1");
    os2 = output_create("S2");
    oand = output_create("AND");

    bitswitch_connect_out(s1, and1, (void*)&bitand_update_ina);
    bitswitch_connect_out(s2, and1, (void*)&bitand_update_inb);
    bitand_connect_out(and1, oand, (void*)&output_update);

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 0);

    output_print(os1);
    output_print(os2);
    output_println(oand);

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 0);

    output_print(os1);
    output_print(os2);
    output_println(oand);

    bitswitch_setval(s1, 0);
    bitswitch_setval(s2, 1);

    output_print(os1);
    output_print(os2);
    output_println(oand);

    bitswitch_setval(s1, 1);
    bitswitch_setval(s2, 1);

    output_print(os1);
    output_print(os2);
    output_println(oand);

    return 0;
}
