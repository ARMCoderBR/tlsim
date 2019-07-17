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
void output_connect(output *o, char *name, int *getvalue){

    if (name){
        o->name = malloc(strlen(name)+1);
        strcpy(o->name,name);
    }
    else
        o->name = NULL;

    o->getval = getvalue;
}

////////////////////////////////////////////////////////////////////////////////
void output_print(output *o){

    if (o->name)
        printf("%s:%d ",o->name,*o->getval);
    else
        printf("%d ",*o->getval);
}

////////////////////////////////////////////////////////////////////////////////
void output_println(output *o){

    output_print(o);
    printf("\r\n");
}
