/*
 * ram_ctl.h
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#ifndef RAM_CTL_H_
#define RAM_CTL_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "bitswitch.h"
#include "ls157.h"
#include "ls173.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    ls157 *ls157_addr;
    ls173 *ls173_addreg;
    ls157 *ls157_datalo, *ls157_datahi;
    ls157 *ls157_write;
    indicator *ledaddr[4];
    bitswitch *run_prog;
    bitswitch *progaddr[4];
    bitswitch *progdata[8];
    bitswitch *progwrite;
    char name[30];

} ram_ctl;

ram_ctl *ram_ctl_create(char *name);

void ram_ctl_connect_bit_out (ram_ctl *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp));

void ram_ctl_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_ctl *dest,int index);

void ram_ctl_in_addr_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_ctl *dest,int index);

void ram_ctl_in_we_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from,ram_ctl *dest);

void ram_ctl_in_oe_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_ctl *dest);



///////////////////////////////

extern void (*ram_ctl_in_dataN[])(void *dest, int *valptr, int timestamp);

void ram_ctl_in_data0(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data1(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data2(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data3(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data4(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data5(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data6(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_data7(ram_ctl *dest, int *valptr, int timestamp);


extern void (*ram_ctl_in_addrN[])(void *dest, int *valptr, int timestamp);

void ram_ctl_in_addr0(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_addr1(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_addr2(ram_ctl *dest, int *valptr, int timestamp);
void ram_ctl_in_addr3(ram_ctl *dest, int *valptr, int timestamp);


board_object *ram_ctl_board_create(ram_ctl *reg, int key, char *name); // Requer NCURSES

#endif /* RAM_CTL_H_ */
