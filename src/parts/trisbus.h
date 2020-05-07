/*
 * trisbus.h
 *
 *  Created on: 21 de jul de 2019
 *      Author: milton
 */

#ifndef TRISBUS_H_
#define TRISBUS_H_

#include "update.h"

#define TRISBUS_BRANCHES 10

typedef struct {

    void (*destroy)(void **dest);
    int in[TRISBUS_BRANCHES];
    int inqty;
    int oldvalue;
    int value;
    ehandler *out_event_handler_root;
} trisnode_s;

typedef struct {

    trisnode_s *trisnode;
    int nlines;
} trisbus;

trisbus *trisbus_create(int nlines);

void trisbus_destrou(trisnode_s **dest);

#endif /* TRISBUS_H_ */
