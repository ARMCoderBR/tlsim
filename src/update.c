/*
 * update.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "update.h"

#define EVQUEUELEN 2000

event evqueue[EVQUEUELEN];
int evinq = 0;
int evins = 0;
int evget = 0;

void event_insert(event *e);

////////////////////////////////////////////////////////////////////////////////
void event_process(){

#ifdef DEBUG
    printf("event_process BEGIN evins:%d evget:%d\n",evins,evget);
#endif

    ehandler * ehandlerptr;
    while (evinq){

        ehandlerptr = evqueue[evget].event_handler_root;
        int *valueptr = evqueue[evget].valueptr;
        int timestamp = 1 + evqueue[evget].timestamp;

        evget++;
        evget %= EVQUEUELEN;
        --evinq;

        while (ehandlerptr != NULL){

            ehandlerptr->objdest_event_handler(ehandlerptr->objdest,valueptr,timestamp);
            ehandlerptr = ehandlerptr->next;
        }
    }

#ifdef DEBUG
    printf("event_process END\n");
#endif
}

////////////////////////////////////////////////////////////////////////////////
void event_insert(event *e){

#ifdef DEBUG
    printf("event_insert BEGIN\n");
#endif

    if (evinq == EVQUEUELEN){

        perror ("Event queue FULL\n");
        exit(-1);
    }

    evinq++;

    memcpy(&evqueue[evins],e,sizeof(event));
    evins++;
    evins %= EVQUEUELEN;

    event_process();

#ifdef DEBUG
    printf("event_insert END evins:%d evget:%d\n",evins,evget);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void new_ehandler(ehandler **ehptr, void *objdest, void (*objdest_event_handler)(void *objdest, int *valptr, int timestamp)){

    ehandler *ept = malloc(sizeof(ehandler));
    ept->objdest_event_handler = objdest_event_handler;
    ept->objdest = objdest;
    ept->next = NULL;

    if (*ehptr == NULL){

        *ehptr = ept;
#ifdef DEBUG
        printf("new_ehandler(1)\n");
#endif
        return;
    }

    ehandler *eptscan = *ehptr;

    while (eptscan->next != NULL)
        eptscan = eptscan->next;

#ifdef DEBUG
    printf("new_ehandler(2)\n");
#endif

    eptscan->next = ept;
}

////////////////////////////////////////////////////////////////////////////////
int update_val_multi(vallist **rootptr, int *valptr){

    vallist *entryptr = *rootptr;

    if (!entryptr){

        entryptr = malloc(sizeof(vallist));
        if (!entryptr){

            perror("update_val_multi cant malloc()\n");
            exit(-1);
        }

        entryptr->valptr = valptr;
        entryptr->next = NULL;
        *rootptr = entryptr;
        return *valptr;
    }

    int val = 2;
    int found = 0;
    vallist *lastptr = NULL;

    while (entryptr){

        if (*entryptr->valptr < val)
            val = *entryptr->valptr;

        if (entryptr->valptr == valptr)
            found = 1;

        lastptr = entryptr;
        entryptr = entryptr->next;
    }

    if (!found){

        if (*valptr < val)
            val = *valptr;

        vallist *newptr = malloc(sizeof(vallist));
        if (!newptr){
            perror ("update_val_multi() cant malloc()\n");
            exit(-1);
        }

        newptr->valptr = valptr;
        newptr->next = NULL;
        lastptr->next = newptr;
    }

    return val;
}

