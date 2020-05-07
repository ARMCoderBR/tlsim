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

    event e;

    for (;s->running;){

        if (s->out_event_handler_root == NULL){

            usleep(20000);
            continue;
        }

        board_mutex_lock();
        s->value = 1;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(&e);
        board_mutex_unlock();
        usleep(10000);

        board_mutex_lock();
        s->value = 0;
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(&e);
        board_mutex_unlock();
        usleep(10000);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
clkgen *clkgen_create(char *name){

    clkgen *b = malloc(sizeof(clkgen));

    if (b == NULL)
        return NULL;

    b->out_event_handler_root = NULL;

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

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

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_connect_out(clkgen *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    new_ehandler(&source->out_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->value,0);
}

