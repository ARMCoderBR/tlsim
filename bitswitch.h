/*
 * bitswitch.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef BITSWITCH_H_
#define BITSWITCH_H_

typedef struct{

    int oldvalue;
    int value;
} bitswitch;

void bitswitch_init(bitswitch *s, int val);

void bitswitch_setval(bitswitch *s, int val);

int bitswitch_getval(bitswitch *s);

#endif /* BITSWITCH_H_ */
