////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <stdlib.h>

#include "update.h"
#include "ls76.h"

// DUAL J-K FLIP-FLOPS WITH PRESET AND CLEAR

////////////////////////////////////////////////////////////////////////////////
static void ls76_up(ls76 *a, timevalue_t timestamp, int index){

    event e;
    e.timestamp = timestamp+1;
    int q, qn;

    if (!a->pre[index] && !a->clr[index]){

        q = qn = 1;
        goto end76;
    }

    if (a->q[index] && a->qn[index]){  // Situação instável gerada por PRE e CLR acionados ao mesmo tempo!
        if (rand() & 1)
            a->q[index] = 0;
        else
            a->qn[index] = 0;
    }

    if (!a->pre[index]){

        q = 1; qn = 0;
        goto end76;
    }

    if (!a->clr[index]){

        q = 0; qn = 1;
        goto end76;
    }

    // Aqui PRE e CLR estão em 1, depende de CLK

    if (!a->clk[index]){

        a->clk_o[index] = a->clk[index];
        return;
    }

    if (!a->clk_o[index]){  //Detectou borda de subida

        a->clk_o[index] = a->clk[index];

        // Vai tratar J e K
        q = a->q[index];
        qn = a->qn[index];

        if (!a->j[index] && !a->k[index]) return;   // Fica como está
        else
        if (a->j[index] && !a->k[index]) { q = 1; qn = 0; }
        else
        if (!a->j[index] && a->k[index]) { q = 0; qn = 1; }
        else
        { q = qn; qn = q ^ 1; } // Toggle
    }

end76:

    if (a->q[index] != q){

        a->q[index] = q;
        e.event_handler_root = a->q_event_handler_root[index];
        e.valueptr = &a->q[index];
        event_insert(&e);
    }

    if (a->qn[index] != qn){

        a->qn[index] = qn;
        e.event_handler_root = a->qn_event_handler_root[index];
        e.valueptr = &a->qn[index];
        event_insert(&e);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
ls76 *ls76_create(){

    ls76 *b = malloc(sizeof(ls76));

    if (b == NULL){
        return NULL;
    }

    int i;
    for (i = 0; i < NUM_BITS_LS76; i++){

        //Entradas
        b->pre[i] = b->clr[i] = b->clk[i] = b->j[i] = b->k[i] = 2;
        b->pre_rootptr[i] = b->clr_rootptr[i] = b->clk_rootptr[i] = b->j_rootptr[i] = b->k_rootptr[i] = NULL;
        //Estado
        b->clk_o[i] = 2;
        //Saídas
        if (rand() & 1)
            {b->q[i] = 0; b->qn[i] = 1;}
        else
            {b->q[i] = 1; b->qn[i] = 0;}
        b->q_event_handler_root[i] = b->qn_event_handler_root[i] = NULL;
    }

    b->destroy = (void*)ls76_destroy;

    return b;
}

////////////////////////////////////////////////////////////////////////////////
void ls76_destroy (ls76 **dest){

    if (dest == NULL) return;
    ls76 *b = *dest;
    if (b == NULL) return;

    int i;
    for (i = 0; i < NUM_BITS_LS76; i++){

        vallist_destroy(&b->pre_rootptr[i]);
        vallist_destroy(&b->clr_rootptr[i]);
        vallist_destroy(&b->clk_rootptr[i]);
        vallist_destroy(&b->j_rootptr[i]);
        vallist_destroy(&b->k_rootptr[i]);
        ehandler_destroy(&b->q_event_handler_root[i]);
        ehandler_destroy(&b->qn_event_handler_root[i]);
    }

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_q(ls76 *source, int index, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->q_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->q[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_qn(ls76 *source, int index, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->qn_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->qn[index],0);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_j(ls76 *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->j_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == dest->j[index]) return;
    dest->j[index] = val;
    ls76_up(dest,timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_k(ls76 *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->k_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == dest->k[index]) return;
    dest->k[index] = val;
    ls76_up(dest,timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_pre(ls76 *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->pre_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == dest->pre[index]) return;
    dest->pre[index] = val;
    ls76_up(dest,timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_clr(ls76 *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->clr_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == dest->clr[index]) return;
    dest->clr[index] = val;
    ls76_up(dest,timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_clk(ls76 *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    bitvalue_t val = update_val_multi(&dest->clk_rootptr[index], valptr);

    if (val > 1) val = 1;

    if (val == dest->clk[index]) return;
    dest->clk[index] = val;
    ls76_up(dest,timestamp, index);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_1q(ls76 *source, void *dest, event_function_t dest_event_handler){

    ls76_connect_q(source, 0, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_1qn(ls76 *source, void *dest, event_function_t dest_event_handler){

    ls76_connect_qn(source, 0, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_1j(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_j(dest, 0, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_1k(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_k(dest, 0, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_1pre(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_pre(dest, 0, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_1clr(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_clr(dest, 0, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_1clk(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_clk(dest, 0, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_2q(ls76 *source, void *dest, event_function_t dest_event_handler){

    ls76_connect_q(source, 1, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_connect_2qn(ls76 *source, void *dest, event_function_t dest_event_handler){

    ls76_connect_qn(source, 1, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_2j(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_j(dest, 1, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_2k(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_k(dest, 1, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_2pre(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_pre(dest, 1, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_2clr(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_clr(dest, 1, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ls76_in_2clk(ls76 *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls76_in_clk(dest, 1, valptr, timestamp);
}

