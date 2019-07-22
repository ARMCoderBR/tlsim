/*
 * trisbus.c
 *
 *  Created on: 21 de jul de 2019
 *      Author: milton
 */

#include "trisbus.h"

#include <malloc.h>

////////////////////////////////////////////////////////////////////////////////
trisbus *trisbus_create(int nlines){

    trisbus *b = malloc(sizeof(trisbus));
    b->trisnode = malloc(nlines * sizeof(trisnode_s));

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void trisbus_connect_out(trisbus *a, int index, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    new_ehandler(&a->trisnode[index].out_event_handler_root, obj, event_handler);
    event_handler(obj,a->trisnode[index].value,0);
}
