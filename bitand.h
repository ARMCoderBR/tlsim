/*
 * bitand.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef BITAND_H_
#define BITAND_H_

#include "update.h"

typedef struct {

    int ina;
    int inb;
    int oldvalue;
    int value;
    ehandler *out_event_handler_root;
} bitand;

bitand *bitand_create();

void bitand_connect_out(bitand *a, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

void bitand_update_ina(bitand *a, int val, int timestamp);
void bitand_update_inb(bitand *a, int val, int timestamp);

#endif /* BITAND_H_ */
