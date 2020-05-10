/*
 * bitconst.h
 *
 *  Created on: 29 de mar de 2020
 *      Author: milton
 */

#ifndef BITCONST_H_
#define BITCONST_H_

#include "update.h"

void bitconst_connect_one(void *dest, event_function_t dest_event_handler);
void bitconst_connect_zero(void *dest, event_function_t dest_event_handler);

#endif /* BITSWITCH_H_ */
