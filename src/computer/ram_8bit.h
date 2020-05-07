/*
 * ram_8bit.h
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

#ifndef RAM_8BIT_H_
#define RAM_8BIT_H_

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "bitswitch.h"
#include "ls04.h"
#include "ls157.h"
#include "ls173.h"
#include "ls189.h"
#include "ls245.h"
#include "ls00.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    void (*destroy)(void **dest);
    ls189 *ls189_hi, *ls189_lo;
    ls04 *ls04_hi, *ls04_lo;
    ls245 *ls245_1;
    indicator *leddata[8];
    indicator *ledprog;
    indicator *ledrun;
    //////////////
    ls157 *ls157_addr;
    ls173 *ls173_addreg;
    ls157 *ls157_datalo, *ls157_datahi;
    ls157 *ls157_write;
    ls00 *ls00_clk;
    indicator *ledaddr[4];
    bitswitch *prog_run;
    bitswitch *progaddr[4];
    bitswitch *progdata[8];
    bitswitch *progwrite;
    int oldclk;
    int clk;
    vallist *clk_rootptr;
    int valfwd;
    char name[30];

} ram_8bit;

ram_8bit *ram_8bit_create(char *name);

void ram_8bit_destroy(ram_8bit **dest);

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

void ram_8bit_in_wdata(ram_8bit *dest, int *valptr, int timestamp);

void ram_8bit_in_waddr(ram_8bit *dest, int *valptr, int timestamp);

void ram_8bit_in_oe(ram_8bit *dest, int *valptr, int timestamp);

void ram_8bit_in_clk(ram_8bit *dest, int *valptr, int timestamp);

board_object *ram_8bit_board_create(ram_8bit *reg, int key, char *name); // Requer NCURSES

#endif /* RAM_8BIT_H_ */
