/*
 * interactive.h
 *
 *  Created on: 28 de mar de 2020
 *      Author: milton
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "bitswitch.h"
#include "indicator.h"

int board_init(int width, int height);

int board_add_manual_switch(bitswitch *bs, int pos_w, int pow_h, int key, char *name);

int board_add_led(indicator *out, int pos_w, int pos_h, char *name);

int board_add_xdigit(indicator *out, int pos_w, int pos_h, char *name);

int board_assign_clock_to_switch(bitswitch *bs);

void board_run();


#endif /* BOARD_H_ */
