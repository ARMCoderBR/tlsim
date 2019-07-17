/*
 * update.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef UPDATE_H_
#define UPDATE_H_

typedef enum {

    BITAND,
} objtype;

typedef struct {

    int type;
    void *ptr;
} object;

typedef struct {

    void (*event_handler)(void *obj, int val, int timestamp);
    void *obj;
    void *next;
} ehandler;

typedef struct {

    ehandler *event_handler_root;
    int value;
    int timestamp;
} event;

void event_insert(event *e);

void update_register (void *o, int type);

void update_run();

void new_ehandler(ehandler *ehptr, void *obj, void (*event_handler)(void *obj, int val, int timestamp));

#endif /* UPDATE_H_ */
