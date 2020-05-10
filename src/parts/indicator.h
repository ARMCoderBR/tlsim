////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include "update.h"

typedef struct  {

    part_destroy_function_t destroy;
    char name[30];
    vallist *ind0_rootptr, *ind1_rootptr, *ind2_rootptr, *ind3_rootptr;
    uint8_t value;
    uint8_t oldvalue;
    bool_t refreshable;
} indicator;

indicator *indicator_create(char *name);

void indicator_destroy(indicator **dest);

void indicator_in_d0(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp);
void indicator_in_d1(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp);
void indicator_in_d2(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp);
void indicator_in_d3(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp);

void indicator_print(indicator *o);
void indicator_println(indicator *o);
void indicator_prints(indicator *o);

#endif /* INDICATOR_H_ */
