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

////////////////////////////////////////////////////////////////////////////////
void event_init(event_context_t *ec){

    ec->event_list = NULL;
    ec->event_last = NULL;
    ec->scanning_timestamp = 0;
    ec->max_timestamp = 0;
    ec->logfile = NULL;
    ec->logging = 0;
    pthread_mutex_init(&ec->event_mutex,NULL);
}

////////////////////////////////////////////////////////////////////////////////
void event_flush(event_context_t *ec){

    if (ec->max_timestamp || ec->event_list)
        logger(ec, "== event_flush() max_timestamp:%d",ec->max_timestamp);

    while (ec->event_list){

        ec->event_last = ec->event_list;
        ec->event_list = ec->event_list->next;
        free(ec->event_last);
    }

    ec->event_last = NULL;
    ec->scanning_timestamp = 0;
    ec->max_timestamp = 0;
}

////////////////////////////////////////////////////////////////////////////////
void event_mutex_lock(event_context_t *ec){

    pthread_mutex_lock(&ec->event_mutex);
}

////////////////////////////////////////////////////////////////////////////////
void event_mutex_unlock(event_context_t *ec){

    pthread_mutex_unlock(&ec->event_mutex);
}

////////////////////////////////////////////////////////////////////////////////
bool_t event_process(event_context_t *ec){

#ifdef DEBUG
    printf("event_process BEGIN evins:%d evget:%d\n",evins,evget);
#endif

    if (!ec->event_list) return 0;

    while (ec->scanning_timestamp <= ec->max_timestamp){

        event * eventptr = ec->event_list;
        bool_t found = 0;

        while (eventptr){

            if ((eventptr->timestamp == ec->scanning_timestamp) && (!eventptr->done)){

                found = 1;
                eventptr->done = 1;

                ehandler * ehandlerptr = eventptr->event_handler_root;

                //int *valueptr = eventptr->valueptr;
                //int timestamp = 1 + eventptr->timestamp;
                int seq=0;
                while (ehandlerptr != NULL){

                    logger(ec, "### run(%d) valueptr:%p *valueptr:%d TS:%d",seq++,eventptr->valueptr,*eventptr->valueptr,eventptr->timestamp);
                    ehandlerptr->objdest_event_handler(ehandlerptr->objdest,eventptr->valueptr,eventptr->timestamp);
                    ehandlerptr = ehandlerptr->next;
                }
            }

            eventptr = eventptr->next;
        }

        if (!found)
            ec->scanning_timestamp++;
    }

    event_flush(ec);

#ifdef DEBUG
    printf("event_process END\n");
#endif

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
void event_insert(event_context_t *ec, event *e){

#ifdef DEBUG
    printf("event_insert BEGIN\n");
#endif

    event *newev = malloc(sizeof(event));
    if (!newev){

        perror("event_insert() could not malloc()\n");
        exit(-1);
    }

    logger(ec, "\n+++ event_insert valptr:%p *valptr:%d timestamp:%d", e->valueptr, *e->valueptr, e->timestamp);

    memcpy(newev,e,sizeof(event));
    newev->done = 0;
    newev->next = NULL;

    if (!ec->event_list){

        ec->event_list = newev;
        ec->scanning_timestamp = ec->max_timestamp = 0;
    }else
        ec->event_last->next = newev;

    if (newev->timestamp > ec->max_timestamp)
        ec->max_timestamp = newev->timestamp;

    ec->event_last = newev;

#ifdef DEBUG
    printf("event_insert END\n");
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
void logger_init(event_context_t *ec){

    if (!ec->logging) return;

    ec->logfile = fopen ("out.log","w");
    logger(ec, "=============================");
    logger(ec, "======= SESSION START =======");
    logger(ec, "=============================");
}

////////////////////////////////////////////////////////////////////////////////
void logger(event_context_t *ec, const char *fmt, ...) {

    if (!ec->logging) return;

    char buftxt[301];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buftxt, 300, fmt, args);
    va_end(args);
    fprintf(ec->logfile,"%s\n",buftxt);
    fflush(ec->logfile);
}

////////////////////////////////////////////////////////////////////////////////
void logger_end(event_context_t *ec){

    if (!ec->logging) return;

    if (ec->logfile) fclose(ec->logfile);
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
