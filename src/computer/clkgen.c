/*
 * clkgen.c
 *
 *  Created on: 2 de mai de 2020
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "clkgen.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
void *clkgen_thread(void *args){

    clkgen *s = (clkgen*)args;

    s->running = 1;

    int sleep_us = s->period_us / 2;

    event e;

    for (;s->running;){

        if (s->out_event_handler_root == NULL){

            usleep(200000);
            continue;
        }

        if (s->halt){

            usleep(sleep_us);
            continue;
        }

        board_mutex_lock();
        s->value = 1;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(&e);
        board_mutex_unlock();
        usleep(sleep_us);

        board_mutex_lock();
        s->value = 0;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(&e);
        board_mutex_unlock();
        usleep(sleep_us);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
clkgen *clkgen_create(char *name, int period_us){

    clkgen *b = malloc(sizeof(clkgen));

    if (b == NULL)
        return NULL;

    b->halt = 1;
    b->halt_rootptr = NULL;

    b->out_event_handler_root = NULL;

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

    b->period_us = period_us;

    pthread_create(&b->clkthread, NULL, clkgen_thread, b);

    b->destroy = (void*)clkgen_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_destroy (clkgen **dest){

    if (dest == NULL) return;
    clkgen *b = *dest;
    if (b == NULL) return;

    b->running = 0;
    pthread_join(b->clkthread, NULL);

    ehandler_destroy(&b->out_event_handler_root);
    vallist_destroy(&b->halt_rootptr);

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_in_halt(clkgen *dest, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->halt_rootptr, valptr);

    if (val > 1) val = 1;

    dest->halt = val;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_connect_out(clkgen *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->value,0);
}

