////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2021 BY ARMCODER - milton@armcoder.com.br
//
// https://github.com/ARMCoderBR/tlsim
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef UPDATE_H_
#define UPDATE_H_

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

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

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    event *event_list;
    event *event_last;
    int scanning_timestamp;
    int max_timestamp;
    FILE *logfile;
    bool_t logging;
    pthread_mutex_t event_mutex;
    void *bctx;
} event_context_t;

event_context_t *event_init(void);

void event_insert(event_context_t *ec, event *e);

void event_mutex_lock(event_context_t *ec);
void event_mutex_unlock(event_context_t *ec);
bool_t event_process(event_context_t *ec);

void new_ehandler(ehandler **ehptr, void *objdest, event_function_t objdest_event_handler);

bitvalue_t update_val_multi(vallist **rootptr, bitvalue_t *valptr);

////////////////////////////////////////////////////////////////////////////////
void logger_init(event_context_t *ec);
void logger(event_context_t *ec, const char *fmt, ...);
void logger_end(event_context_t *ec);

void vallist_destroy(vallist **root);

void ehandler_destroy(ehandler **root);


////////////////////////////////////////////////////////////////////////////////
typedef void (*part_destroy_function_t)(void **dest);
void part_destroy(void **part);

#define DESTROY(X) part_destroy((void**)&X)

#define SW_VERSION  1
#define SW_REVISION 1
#define SW_MINOR    3

#endif /* UPDATE_H_ */
