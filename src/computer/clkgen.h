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

    int value;
    ehandler *out_event_handler_root;
    pthread_t clkthread;
    int running;
    char name[30];
} clkgen;

clkgen *clkgen_create(char *name);

void clkgen_destroy (clkgen **dest);

void clkgen_connect_out(clkgen *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

#endif /* SRC_COMPUTER_CLKGEN_H_ */
