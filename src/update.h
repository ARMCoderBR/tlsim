/*
 * update.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef UPDATE_H_
#define UPDATE_H_

typedef struct {

    int type;
    void *ptr;
} object;

typedef struct {

    int *valptr;
    void *next;
} vallist;

typedef struct {

    void (*objdest_event_handler)(void *objdest, int *valptr, int timestamp);
    void *objdest;
    void *next;
} ehandler;

typedef struct {

    ehandler *event_handler_root;
    int *valueptr;
    int timestamp;
    int done;
    void *next;
} event;

void event_insert(event *e);

void update_register (void *o, int type);

void update_run();

int event_process();

void new_ehandler(ehandler **ehptr, void *objdest, void (*objdest_event_handler)(void *objdest, int *valptr, int timestamp));

int update_val_multi(vallist **rootptr, int *valptr);

////////////////////////////////////////////////////////////////////////////////
void logger_init();
void logger(const char *fmt, ...);
void logger_end();

#endif /* UPDATE_H_ */
