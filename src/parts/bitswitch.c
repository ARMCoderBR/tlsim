////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <string.h>

#include "bitswitch.h"
#include "update.h"

////////////////////////////////////////////////////////////////////////////////
bitswitch *bitswitch_create(event_context_t *ec, char *name){

    bitswitch *b = malloc(sizeof(bitswitch));

    if (b == NULL)
        return NULL;

    b->ec = ec;
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

        logger(s->ec, "\n== bitswitch_setval [%s] val:%d",s->name,val);

        s->oldvalue = s->value;
        event e;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(s->ec, &e);
        event_mutex_lock(s->ec);
        while (event_process(s->ec));
        event_mutex_unlock(s->ec);

    }
}
