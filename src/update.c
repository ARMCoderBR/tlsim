////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include "update.h"

//#define EVQUEUELEN 2000

//event evqueue[EVQUEUELEN];
//int evinq = 0;
//int evins = 0;
//int evget = 0;

void event_insert(event *e);

event *event_list = NULL;
event *event_last = NULL;
int scanning_timestamp = 0;
int max_timestamp = 0;

////////////////////////////////////////////////////////////////////////////////
void event_flush(){

    if (max_timestamp || event_list)
        logger("== event_flush() max_timestamp:%d",max_timestamp);

    while (event_list){

        event_last = event_list;
        event_list = event_list->next;
        free(event_last);
    }

    event_last = NULL;
    scanning_timestamp = 0;
    max_timestamp = 0;
}

////////////////////////////////////////////////////////////////////////////////
bool_t event_process(){

#ifdef DEBUG
    printf("event_process BEGIN evins:%d evget:%d\n",evins,evget);
#endif

    if (!event_list) return 0;

    while (scanning_timestamp <= max_timestamp){

        event * eventptr = event_list;
        bool_t found = 0;

        while (eventptr){

            if ((eventptr->timestamp == scanning_timestamp) && (!eventptr->done)){

                found = 1;
                eventptr->done = 1;

                ehandler * ehandlerptr = eventptr->event_handler_root;

                //int *valueptr = eventptr->valueptr;
                //int timestamp = 1 + eventptr->timestamp;
                int seq=0;
                while (ehandlerptr != NULL){

                    logger("### run(%d) valueptr:%p *valueptr:%d TS:%d",seq++,eventptr->valueptr,*eventptr->valueptr,eventptr->timestamp);
                    ehandlerptr->objdest_event_handler(ehandlerptr->objdest,eventptr->valueptr,eventptr->timestamp);
                    ehandlerptr = ehandlerptr->next;
                }
            }

            eventptr = eventptr->next;
        }

        if (!found)
            scanning_timestamp++;
    }

    event_flush();

#ifdef DEBUG
    printf("event_process END\n");
#endif

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
void event_insert(event *e){

#ifdef DEBUG
    printf("event_insert BEGIN\n");
#endif

    event *newev = malloc(sizeof(event));
    if (!newev){

        perror("event_insert() could not malloc()\n");
        exit(-1);
    }

    logger("\n+++ event_insert valptr:%p *valptr:%d timestamp:%d", e->valueptr, *e->valueptr, e->timestamp);

    memcpy(newev,e,sizeof(event));
    newev->done = 0;
    newev->next = NULL;

    if (!event_list){

        event_list = newev;
        scanning_timestamp = max_timestamp = 0;
    }else
        event_last->next = newev;

    if (newev->timestamp > max_timestamp)
        max_timestamp = newev->timestamp;

    event_last = newev;

    //event_process();

#ifdef DEBUG
    printf("event_insert END evins:%d evget:%d\n",evins,evget);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void new_ehandler(ehandler **ehptr, void *objdest, event_function_t objdest_event_handler){

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
bitvalue_t update_val_multi(vallist **rootptr, bitvalue_t *valptr){

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

    bitvalue_t val = 2;
    bool_t found = 0;
    bool_t found_pulldown = 0;
    vallist *lastptr = NULL;

    while (entryptr){

    	if (*entryptr->valptr == 3)
    		found_pulldown = 1;

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

    if ((val == 2) && (found_pulldown))
    	val = 0;

    return val;
}

////////////////////////////////////////////////////////////////////////////////
FILE *logfile = NULL;

bool_t logging = 0;

void logger_init(){

    if (!logging) return;

    logfile = fopen ("out.log","w");
    logger("=============================");
    logger("======= SESSION START =======");
    logger("=============================");
}

////////////////////////////////////////////////////////////////////////////////
void logger(const char *fmt, ...) {

    if (!logging) return;

    char buftxt[301];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buftxt, 300, fmt, args);
    va_end(args);
    fprintf(logfile,"%s\n",buftxt);
    fflush(logfile);
}

////////////////////////////////////////////////////////////////////////////////
void logger_end(){

    if (!logging) return;

    if (logfile) fclose(logfile);
}

////////////////////////////////////////////////////////////////////////////////
void vallist_destroy(vallist **root){

	if (!root) return;
	vallist *p = *root;
	vallist *old;

	while (p){

		old = p;
		p = p->next;
        free(old);
	}

	*root = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ehandler_destroy(ehandler **root){

	if (!root) return;
	ehandler *p = *root;
	ehandler *old;

	while (p){

		old = p;
		p = p->next;
        free(old);
	}

	*root = NULL;
}

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    part_destroy_function_t destroy;
} part_descriptor;

////////////////////////////////////////////////////////////////////////////////
void part_destroy(void **part){

    if (part == NULL) return;
    part_descriptor *p = *part;
    if (p == NULL) return;

    p->destroy((void*)&p);

    *part = NULL;
}
