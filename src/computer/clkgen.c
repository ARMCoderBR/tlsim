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

    s->value = 1;

    for (;s->running;){

        if (s->out_event_handler_root == NULL){

            usleep(200000);
            continue;
        }

        if (s->halt){

            usleep(200000);
            continue;
        }

        if (s->pause){

            if (s->step)
                s->step = 0;
            else{

                usleep(10000);
                continue;
            }
        }

        event_mutex_lock(s->ec);
        e.event_handler_root = s->out_event_handler_root;
        e.valueptr = &s->value;
        e.timestamp = 0;
        event_insert(s->ec, &e);

        s->valuen = s->value ^1;
        e.event_handler_root = s->outn_event_handler_root;
        e.valueptr = &s->valuen;
        e.timestamp = 0;
        event_insert(s->ec, &e);
        event_mutex_unlock(s->ec);

        if (!s->pause)
            usleep(s->period_us >> 1);
        else
            usleep(20000);

        s->value ^= 1;
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
clkgen *clkgen_create(event_context_t *ec, char *name, int period_us){

    clkgen *b = malloc(sizeof(clkgen));

    if (b == NULL)
        return NULL;

    b->ec = ec;

    b->halt = 2;
    b->halt_rootptr = NULL;

    b->pause = 0;

    b->out_event_handler_root = NULL;
    b->outn_event_handler_root = NULL;

    if (name)
        strncpy(b->name,name,sizeof(b->name));
    else
        b->name[0] = 0;

    b->period_us = period_us;

    b->value = 0; b->valuen = 1;

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
    ehandler_destroy(&b->outn_event_handler_root);
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

////////////////////////////////////////////////////////////////////////////////
void clkgen_connect_outn(clkgen *source, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->outn_event_handler_root, dest, dest_event_handler);
    dest_event_handler(dest,&source->valuen,0);
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_pause(clkgen *dest, bool_t status){

    if (dest->pause == status) return;

    dest->pause = status;

    dest->step = 0;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_step(clkgen *dest){

    dest->step = 1;
}

////////////////////////////////////////////////////////////////////////////////
void clkgen_set_us(clkgen *dest, uint32_t us){

    dest->period_us = us;
}
