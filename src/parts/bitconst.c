/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "bitconst.h"


int bit_one = 1;
int bit_zero = 0;

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_one(void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    dest_event_handler(dest,&bit_one,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_zero(void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    dest_event_handler(dest,&bit_zero,0);
}
