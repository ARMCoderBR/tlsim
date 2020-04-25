/*
 * ram_8bit.h
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#ifndef RAM_8BIT_H_
#define RAM_8BIT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "ls04.h"
#include "ls189.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    ls189 *ls189_hi, *ls189_lo;
    ls04 *ls04_hi, *ls04_lo;
    ls245 *ls245_1;
    indicator *led[8];
    indicator *ledclk;
    char name[30];

} ram_8bit;

ram_8bit *ram_8bit_create(char *name);

void ram_8bit_connect_bit_out (ram_8bit *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ram_8bit_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_8bit *dest,int index);

void ram_8bit_in_addr_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_8bit *dest,int index);

void ram_8bit_in_we_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_8bit *dest);

void ram_8bit_in_oe_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_8bit *dest);



///////////////////////////////

extern void (*ram_8bit_in_dataN[])(void *dest, int *valptr, int timestamp);

void ram_8bit_in_data0(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data1(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data2(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data3(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data4(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data5(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data6(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_data7(ram_8bit *dest, int *valptr, int timestamp);


extern void (*ram_8bit_in_addrN[])(void *dest, int *valptr, int timestamp);

void ram_8bit_in_addr0(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_addr1(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_addr2(ram_8bit *dest, int *valptr, int timestamp);
void ram_8bit_in_addr3(ram_8bit *dest, int *valptr, int timestamp);


board_object *ram_8bit_board_create(ram_8bit *reg, int key, char *name); // Requer NCURSES

#endif /* RAM_8BIT_H_ */
