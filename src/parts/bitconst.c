/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "bitconst.h"


bitvalue_t bit_one = 1;
bitvalue_t bit_zero = 0;

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_one(void *dest, event_function_t dest_event_handler){

    dest_event_handler(dest,&bit_one,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_zero(void *dest, event_function_t dest_event_handler){

    dest_event_handler(dest,&bit_zero,0);
}
