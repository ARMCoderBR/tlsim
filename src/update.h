/*
 * update.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef UPDATE_H_
#define UPDATE_H_

#include <stdint.h>

typedef uint8_t bitvalue_t;
typedef uint8_t bool_t;
typedef uint32_t timevalue_t;

typedef void (*event_function_t)(void *objdest, bitvalue_t *valptr, timevalue_t timestamp);

typedef struct {

    int type;
    void *ptr;
} object;

typedef struct {

    bitvalue_t *valptr;
    void *next;
} vallist;

typedef struct {

    event_function_t objdest_event_handler;
    void *objdest;
    void *next;
} ehandler;

typedef struct {

    ehandler *event_handler_root;
    bitvalue_t *valueptr;
    timevalue_t timestamp;
    bool_t done;
    void *next;
} event;

void event_insert(event *e);

void update_register (void *o, int type);

void update_run();

bool_t event_process();

void new_ehandler(ehandler **ehptr, void *objdest, event_function_t objdest_event_handler);

bitvalue_t update_val_multi(vallist **rootptr, bitvalue_t *valptr);

////////////////////////////////////////////////////////////////////////////////
void logger_init();
void logger(const char *fmt, ...);
void logger_end();

void vallist_destroy(vallist **root);

void ehandler_destroy(ehandler **root);


////////////////////////////////////////////////////////////////////////////////
typedef void (*part_destroy_function_t)(void **dest);
void part_destroy(void **part);

#define DESTROY(X) part_destroy((void**)&X)

#endif /* UPDATE_H_ */
