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

    if (!nlines) return NULL;

    trisbus *b = malloc(sizeof(trisbus));

    if (b == NULL)
        return NULL;

    b->trisnode = malloc(nlines * sizeof(trisnode_s));

    if (b->trisnode == NULL){

        free(b);
        return NULL;
    }

    b->nlines = nlines;
    return b;
}

////////////////////////////////////////////////////////////////////////////////
void trisbus_connect_out(trisbus *source, int index, void *dest, void (*dest_event_handler)(void *dest, int val, int timestamp)){

    new_ehandler(&source->trisnode[index].out_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,source->trisnode[index].value,0);
}
