/*
 * update.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>

#include "update.h"
#include "bitand.h"

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

////////////////////////////////////////////////////////////////////////////////
void event_insert(event *e){

}

////////////////////////////////////////////////////////////////////////////////
void new_ehandler(ehandler *ehptr, void *obj, void (*event_handler)(void *obj, int val, int timestamp)){

    if (ehptr == NULL){

        ehptr = malloc(sizeof(ehandler));
        ehptr->event_handler = event_handler;
        ehptr->obj = obj;
        ehptr->next = NULL;
        return;
    }

    while (ehptr->next != NULL)
        ehptr = ehptr->next;

    ehptr->next = malloc(sizeof(ehandler));
    ehptr = ehptr->next;
    ehptr->event_handler = event_handler;
    ehptr->next = NULL;
}
