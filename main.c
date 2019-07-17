/*
 * main.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include <stdio.h>

#include "bitswitch.h"
#include "output.h"
#include "and.h"
#include "update.h"

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){

    bitswitch s1,s2;
    bitand and1;
    output os1,os2,oand;

    bitswitch_init(&s1,0);
    bitswitch_init(&s2,0);

    bitand_connect(&and1,&s1.value,&s2.value);

    output_connect(&os1,"S1",&s1.value);
    output_connect(&os2,"S2",&s2.value);
    output_connect(&oand,"AND",&and1.value);

    update_register(&and1,BITAND);

    bitswitch_setval(&s1, 0);
    bitswitch_setval(&s2, 0);

    output_print(&os1);
    output_print(&os2);
    output_println(&oand);

    bitswitch_setval(&s1, 1);
    bitswitch_setval(&s2, 0);

    output_print(&os1);
    output_print(&os2);
    output_println(&oand);

    bitswitch_setval(&s1, 0);
    bitswitch_setval(&s2, 1);

    output_print(&os1);
    output_print(&os2);
    output_println(&oand);

    bitswitch_setval(&s1, 1);
    bitswitch_setval(&s2, 1);

    output_print(&os1);
    output_print(&os2);
    output_println(&oand);

    return 0;
}
