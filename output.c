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

    return o;
}

////////////////////////////////////////////////////////////////////////////////
void output_update(output *o, int val, int timestamp){

#ifdef DEBUG
    printf("output_update val:%d\n",val);
#endif
    o->value = val;
}

////////////////////////////////////////////////////////////////////////////////
void output_print(output *o){

    if (o->name)
        printf("%s:%d ",o->name,o->value);
    else
        printf("%d ",o->value);
}

////////////////////////////////////////////////////////////////////////////////
void output_println(output *o){

    output_print(o);
    printf("\r\n");
}
