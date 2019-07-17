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


void update_register (void *o, int type);

void update_run();

#endif /* UPDATE_H_ */
