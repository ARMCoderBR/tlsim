/*
 * and.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include "and.h"

////////////////////////////////////////////////////////////////////////////////
void bitand_connect(bitand *a, int *ina, int *inb){

    a->ina = ina;
    a->inb = inb;
    a->oldval = 2;
}

////////////////////////////////////////////////////////////////////////////////
int bitand_update(bitand *a){

    a->value = *a->ina & *a->inb;

    if (a->oldval != a->value){

        a->oldval = a->value;
        return 1;
    }

    return 0;
}
