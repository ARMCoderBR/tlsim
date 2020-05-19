////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>

#include "update.h"
#include "ls138.h"

// 3-LINE TO 8-LINE DECODER/DEMULTIPLEXER

////////////////////////////////////////////////////////////////////////////////
static void ls138_up(ls138 *a, timevalue_t timestamp){

    int i;
    for (i = 0; i < NUM_BITS_LS138; i++)
        a->y[i] = 1;

    if ((a->ing2a)||(a->ing2b)||(!a->ing1))
        goto end138;

    uint8_t code = (a->inc?4:0) +
               (a->inb?2:0) +
               (a->ina?1:0);

    a->y[code] = 0;

end138:

    for (i = 0; i < NUM_BITS_LS138; i++)
        if (a->oy[i] != a->y[i]){

            a->oy[i] = a->y[i];
            event e;
            e.event_handler_root = a->y_event_handler_root[i];
            e.valueptr = &a->y[i];
            e.timestamp = timestamp+1;
            event_insert(a->ec, &e);
        }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls138 *ls138_create(event_context_t *ec){

    ls138 *b = malloc(sizeof(ls138));

    if (b == NULL)
        return NULL;

    b->ec = ec;

    int i;
    for (i = 0; i < NUM_BITS_LS138; i++){

        b->y[i] = 1;
        b->oy[i] = 1;
        b->y_event_handler_root[i] = NULL;
    }

    b->ina = b->inb = b->inc = 2;
    b->ina_rootptr = b->inb_rootptr = b->inc_rootptr = NULL;

    b->ing2a = b->ing2b = b->ing1 = 2;
    b->ing2a_rootptr = b->ing2b_rootptr = b->ing1_rootptr = NULL;

    b->destroy = (void*)ls138_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls138_destroy (ls138 **dest){

    if (dest == NULL) return;
    ls138 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_BITS_LS138; i++){

        ehandler_destroy(&b->y_event_handler_root[i]);
    }

    vallist_destroy(&b->ina_rootptr);
    vallist_destroy(&b->inb_rootptr);
    vallist_destroy(&b->inc_rootptr);

    vallist_destroy(&b->ing2a_rootptr);
    vallist_destroy(&b->ing2b_rootptr);
    vallist_destroy(&b->ing1_rootptr);

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y(ls138 *source, void *dest, event_function_t dest_event_handler, int index){

    new_ehandler(&source->y_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->y[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y0(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 0);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y1(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 1);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y2(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 2);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y3(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 3);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y4(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 4);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y5(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 5);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y6(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 6);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_connect_y7(ls138 *source, void *dest, event_function_t dest_event_handler){

    ls138_connect_y(source, dest, *dest_event_handler, 7);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_ina(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->ina_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->ina) return;

    a->ina = val;
    ls138_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_inb(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->inb_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->inb) return;

    a->inb = val;
    ls138_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_inc(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->inc_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->inc) return;

    a->inc = val;
    ls138_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_ing2a(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->ing2a_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->ing2a) return;

    a->ing2a = val;
    ls138_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_ing2b(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->ing2b_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->ing2b) return;

    a->ing2b = val;
    ls138_up(a, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls138_ing1(ls138 *a, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&a->ing1_rootptr, valptr);

    if (val > 1) val = 1;

    if (val == a->ing1) return;

    a->ing1 = val;
    ls138_up(a, timestamp);
}
