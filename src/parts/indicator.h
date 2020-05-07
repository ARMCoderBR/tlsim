/*
 * indicator.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef INDICATOR_H_
#define INDICATOR_H_


typedef struct  {

    void (*destroy)(void **dest);
    char name[30];
    vallist *ind0_rootptr, *ind1_rootptr, *ind2_rootptr, *ind3_rootptr;
    int value;
    int oldvalue;
    int refreshable;
} indicator;

indicator *indicator_create(char *name);

void indicator_destroy(indicator **dest);

void indicator_in_d0(indicator *dest, int *valptr, int timestamp);
void indicator_in_d1(indicator *dest, int *valptr, int timestamp);
void indicator_in_d2(indicator *dest, int *valptr, int timestamp);
void indicator_in_d3(indicator *dest, int *valptr, int timestamp);

void indicator_print(indicator *o);
void indicator_println(indicator *o);
void indicator_prints(indicator *o);

#endif /* INDICATOR_H_ */
