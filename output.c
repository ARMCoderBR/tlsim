/*
 * output.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>

#include "output.h"

////////////////////////////////////////////////////////////////////////////////
output *output_create(char *name){

    output *o = malloc(sizeof(output));
    if (name){

        o->name = malloc(1+strlen(name));
        strcpy(o->name, name);
    }
    else
        o->name = NULL;

    o->value = 0;
    return o;
}

////////////////////////////////////////////////////////////////////////////////
void output_update_d0(output *o, int val, int timestamp){

#ifdef DEBUG
    printf("output_update_d0 val:%d\n",val);
#endif
    if (val)
        o->value |= 1;
    else
        o->value &= ~1;
}

////////////////////////////////////////////////////////////////////////////////
void output_update_d1(output *o, int val, int timestamp){

#ifdef DEBUG
    printf("output_update_d1 val:%d\n",val);
#endif
    if (val)
        o->value |= 2;
    else
        o->value &= ~2;
}

////////////////////////////////////////////////////////////////////////////////
void output_update_d2(output *o, int val, int timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif
    if (val)
        o->value |= 4;
    else
        o->value &= ~4;
}

////////////////////////////////////////////////////////////////////////////////
void output_update_d3(output *o, int val, int timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif
    if (val)
        o->value |= 8;
    else
        o->value &= ~8;
}

////////////////////////////////////////////////////////////////////////////////
const char dighex[]="0123456789ABCDEF";
void output_print(output *o){

    if (o->name)
        printf("%s:%c",o->name,dighex[o->value]);
    else
        printf("%c",dighex[o->value]);
}

////////////////////////////////////////////////////////////////////////////////
void output_println(output *o){

    output_print(o);
    printf("\r\n");
}

////////////////////////////////////////////////////////////////////////////////
void output_prints(output *o){

    output_print(o);
    printf(" ");
}
