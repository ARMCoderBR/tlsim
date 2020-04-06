/*
 * indicator.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include "update.h"
#include "indicator.h"

#include <malloc.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
indicator *indicator_create(char *name){

    indicator *o = malloc(sizeof(indicator));

    if (o == NULL)
        return NULL;

    if (name){

        o->name = malloc(1+strlen(name));
        strcpy(o->name, name);
    }
    else
        o->name = NULL;

    o->value = 0;
    o->ind0_rootptr = NULL;
    o->ind1_rootptr = NULL;
    o->ind2_rootptr = NULL;
    o->ind3_rootptr = NULL;

    return o;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d0(indicator *dest, int *valptr, int timestamp){

#ifdef DEBUG
    printf("output_update_d0 val:%d\n",val);
#endif

    int val = update_val_multi(dest->ind0_rootptr, valptr);

    if (val)
        dest->value |= 1;
    else
        dest->value &= ~1;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d1(indicator *dest, int *valptr, int timestamp){

#ifdef DEBUG
    printf("output_update_d1 val:%d\n",val);
#endif

    int val = update_val_multi(dest->ind1_rootptr, valptr);

    if (val)
        dest->value |= 2;
    else
        dest->value &= ~2;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d2(indicator *dest, int *valptr, int timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif

    int val = update_val_multi(dest->ind2_rootptr, valptr);

    if (val)
        dest->value |= 4;
    else
        dest->value &= ~4;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d3(indicator *dest, int *valptr, int timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif

    int val = update_val_multi(dest->ind3_rootptr, valptr);

    if (val)
        dest->value |= 8;
    else
        dest->value &= ~8;
}

////////////////////////////////////////////////////////////////////////////////
const char dighex[]="0123456789ABCDEF";
void indicator_print(indicator *o){

    if (o->name)
        printf("%s:%c",o->name,dighex[o->value]);
    else
        printf("%c",dighex[o->value]);
}

////////////////////////////////////////////////////////////////////////////////
void indicator_println(indicator *o){

    indicator_print(o);
    printf("\r\n");
}

////////////////////////////////////////////////////////////////////////////////
void indicator_prints(indicator *o){

    indicator_print(o);
    printf(" ");
}
