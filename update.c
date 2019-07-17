/*
 * update.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include "update.h"
#include "and.h"

#define MAXPARTS 200

int maxindex = 0;

object objecttable[MAXPARTS];

////////////////////////////////////////////////////////////////////////////////
void update_register (void *o, int type){

    objecttable[maxindex].type = type;
    objecttable[maxindex++].ptr = o;
}

////////////////////////////////////////////////////////////////////////////////
void update_run(){

    int i;

    int nups = 1;
    for (;nups;){

        nups = 0;

        for (i = 0; i < maxindex; i++){

            void *a = objecttable[i].ptr;

            switch(objecttable[i].type){

            case BITAND:
                nups += bitand_update(a);
                break;
            }
        }
    }
}
