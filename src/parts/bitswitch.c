/*
 * bitswitch.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>

#include "bitswitch.h"
#include "update.h"

////////////////////////////////////////////////////////////////////////////////
bitswitch *bitswitch_create(char *name){

    bitswitch *b = malloc(sizeof(bitswitch));

    if (b == NULL)
        return NULL;

    b->oldvalue = 2;
    b->out_event_handler_root = NULL;

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

    b->destroy = (void*)bitswitch_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_destroy(bitswitch **dest){

	if (dest == NULL) return;
	bitswitch *b = *dest;
	if (b == NULL) return;

	ehandler_destroy(&b->out_event_handler_root);

	free(b);
	*dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_connect_out(bitswitch *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->value,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitswitch_setval(bitswitch *s, bitvalue_t val){

    s->value = val;

    if (s->oldvalue != s->value){

        logger("\n== bitswitch_setval [%s] val:%d",s->name,val);

        s->oldvalue = s->value;
        event e;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(&e);
    }
}
