/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include "bitswitch.h"
#include "update.h"

////////////////////////////////////////////////////////////////////////////////
void bitswitch_init(bitswitch *s, int val){

    s->value = val;
    s->oldvalue =  1 - val;
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_setval(bitswitch *s, int val){

    s->value = val;

    if (s->oldvalue != s->value){

        s->oldvalue = s->value;
        update_run();
    }
}

////////////////////////////////////////////////////////////////////////////////
int bitswitch_getval(bitswitch *s){

    return s->value;
}
