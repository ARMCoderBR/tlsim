/*
 * clkgen.h
 *
 *  Created on: 2 de mai de 2020
 *      Author: milton
 */

#ifndef SRC_COMPUTER_CLKGEN_H_
#define SRC_COMPUTER_CLKGEN_H_

#include "update.h"

typedef struct{

    part_destroy_function_t destroy;
    bitvalue_t value;
    ehandler *out_event_handler_root;
    pthread_t clkthread;
    bool_t running;
    bitvalue_t period_us;
    bitvalue_t halt;
    vallist *halt_rootptr;
    char name[30];
} clkgen;

clkgen *clkgen_create(char *name, int period_us);

void clkgen_destroy (clkgen **dest);

void clkgen_in_halt(clkgen *dest, bitvalue_t *valptr, timevalue_t timestamp);

void clkgen_connect_out(clkgen *source, void *dest, event_function_t dest_event_handler);

#endif /* SRC_COMPUTER_CLKGEN_H_ */
