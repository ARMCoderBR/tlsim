/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "bitconst.h"
#include "update.h"


////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_one(void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    dest_event_handler(dest,1,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_zero(void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    dest_event_handler(dest,0,0);
}
