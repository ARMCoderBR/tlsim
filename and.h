/*
 * and.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef AND_H_
#define AND_H_

typedef struct {

    int *ina;
    int *inb;
    int oldval;
    int value;
} bitand;

void bitand_connect(bitand *a, int *ina, int *inb);

int bitand_update(bitand *a);

#endif /* AND_H_ */
