/*
 * ram_8bit.c
 *
 *  Created on: 25 de abr de 2020
 *      Author: milton
 */

// Baseado no projeto do Ben Eater (https://www.youtube.com/watch?v=uYXwCBo40iA)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ram_8bit.h"
#include "bitconst.h"

////////////////////////////////////////////////////////////////////////////////
ram_8bit *ram_8bit_create(char *name){

    ram_8bit *ram = malloc (sizeof(ram_8bit));
    if (!ram) return ram;

    char lshi[60];
    char lslo[60];

    lshi[0] = lslo[0] = 0;
    if (name){

        strncpy(lshi,name,sizeof(lshi));
        strncpy(lslo,name,sizeof(lslo));
    }

    strncat(lshi,"-hiWord",sizeof(lshi)/2);
    strncat(lslo,"-loWord",sizeof(lslo)/2);

    ram->ls189_hi = ls189_create(lshi);
    ram->ls189_lo = ls189_create(lslo);
    ram->ls245_1  = ls245_create();
    ram->ledclk = indicator_create("Clk");

    ram->ls04_hi = ls04_create();
    ram->ls04_lo = ls04_create();

    int i;
    for (i = 0; i < 8; i++){

        strncpy(lshi,name,sizeof(lshi));
        sprintf(lslo,"-D%d",i);
        strncat(lshi,lslo,sizeof(lshi)/2);
        ram->led[i] = indicator_create(lshi);
    }

    ls189_connect_1q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a1);
    ls189_connect_2q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a2);
    ls189_connect_3q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a3);
    ls189_connect_4q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a4);

    ls04_connect_y1(ram->ls04_lo, ram->led[0], (void*)&indicator_in_d0);
    ls04_connect_y2(ram->ls04_lo, ram->led[1], (void*)&indicator_in_d0);
    ls04_connect_y3(ram->ls04_lo, ram->led[2], (void*)&indicator_in_d0);
    ls04_connect_y4(ram->ls04_lo, ram->led[3], (void*)&indicator_in_d0);

    ls189_connect_1q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a1);
    ls189_connect_2q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a2);
    ls189_connect_3q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a3);
    ls189_connect_4q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a4);

    ls04_connect_y1(ram->ls04_hi, ram->led[4], (void*)&indicator_in_d0);
    ls04_connect_y2(ram->ls04_hi, ram->led[5], (void*)&indicator_in_d0);
    ls04_connect_y3(ram->ls04_hi, ram->led[6], (void*)&indicator_in_d0);
    ls04_connect_y4(ram->ls04_hi, ram->led[7], (void*)&indicator_in_d0);

    ls04_connect_y1(ram->ls04_lo, ram->ls245_1, (void*)&ls245_in_a1);
    ls04_connect_y2(ram->ls04_lo, ram->ls245_1, (void*)&ls245_in_a2);
    ls04_connect_y3(ram->ls04_lo, ram->ls245_1, (void*)&ls245_in_a3);
    ls04_connect_y4(ram->ls04_lo, ram->ls245_1, (void*)&ls245_in_a4);

    ls04_connect_y1(ram->ls04_hi, ram->ls245_1, (void*)&ls245_in_a5);
    ls04_connect_y2(ram->ls04_hi, ram->ls245_1, (void*)&ls245_in_a6);
    ls04_connect_y3(ram->ls04_hi, ram->ls245_1, (void*)&ls245_in_a7);
    ls04_connect_y4(ram->ls04_hi, ram->ls245_1, (void*)&ls245_in_a8);

    bitconst_connect_zero(ram->ls189_lo, (void*)&ls189_in_cs);
    bitconst_connect_zero(ram->ls189_hi, (void*)&ls189_in_cs);

    bitconst_connect_one(ram->ls245_1, (void*)&ls245_in_dir);

    if (name)
        strncpy(ram->name,name,sizeof(ram->name));
    else
        ram->name[0] = 0;

    return ram;
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_connect_bit_out (ram_8bit *source, int index, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)){

    switch(index){
        case 0:    ls245_connect_b1(source->ls245_1,dest,dest_event_handler); break;
        case 1:    ls245_connect_b2(source->ls245_1,dest,dest_event_handler); break;
        case 2:    ls245_connect_b3(source->ls245_1,dest,dest_event_handler); break;
        case 3:    ls245_connect_b4(source->ls245_1,dest,dest_event_handler); break;
        case 4:    ls245_connect_b5(source->ls245_1,dest,dest_event_handler); break;
        case 5:    ls245_connect_b6(source->ls245_1,dest,dest_event_handler); break;
        case 6:    ls245_connect_b7(source->ls245_1,dest,dest_event_handler); break;
        case 7:    ls245_connect_b8(source->ls245_1,dest,dest_event_handler); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_data_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_8bit *dest, int index){

    switch(index){
        case 0: connect_fn(from,dest->ls189_lo,(void*)&ls189_in_1d); break;
        case 1: connect_fn(from,dest->ls189_lo,(void*)&ls189_in_2d); break;
        case 2: connect_fn(from,dest->ls189_lo,(void*)&ls189_in_3d); break;
        case 3: connect_fn(from,dest->ls189_lo,(void*)&ls189_in_4d); break;
        case 4: connect_fn(from,dest->ls189_hi,(void*)&ls189_in_1d); break;
        case 5: connect_fn(from,dest->ls189_hi,(void*)&ls189_in_2d); break;
        case 6: connect_fn(from,dest->ls189_hi,(void*)&ls189_in_3d); break;
        case 7: connect_fn(from,dest->ls189_hi,(void*)&ls189_in_4d); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_addr_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_8bit *dest, int index){

    switch(index){
        case 0:
            connect_fn(from,dest->ls189_lo,(void*)&ls189_in_a0);
            connect_fn(from,dest->ls189_hi,(void*)&ls189_in_a0);
            break;
        case 1:
            connect_fn(from,dest->ls189_lo,(void*)&ls189_in_a1);
            connect_fn(from,dest->ls189_hi,(void*)&ls189_in_a1);
            break;
        case 2:
            connect_fn(from,dest->ls189_lo,(void*)&ls189_in_a2);
            connect_fn(from,dest->ls189_hi,(void*)&ls189_in_a2);
            break;
        case 3:
            connect_fn(from,dest->ls189_lo,(void*)&ls189_in_a3);
            connect_fn(from,dest->ls189_hi,(void*)&ls189_in_a3);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_we_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_8bit *dest){

    connect_fn(from,dest->ls189_lo,(void*)&ls189_in_we);
    connect_fn(from,dest->ls189_hi,(void*)&ls189_in_we);
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_oe_from(void (*connect_fn)(void *source, void *dest, void (*dest_event_handler)(void *dest, int *valptr, int timestamp)),
                        void *from, ram_8bit *dest){

    connect_fn(from,dest->ls245_1,(void*)&ls245_in_oe);
}

void ram_8bit_in_data0(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_1d(dest->ls189_lo, valptr, timestamp);
}

void ram_8bit_in_data1(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_2d(dest->ls189_lo, valptr, timestamp);
}

void ram_8bit_in_data2(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_3d(dest->ls189_lo, valptr, timestamp);
}

void ram_8bit_in_data3(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_4d(dest->ls189_lo, valptr, timestamp);
}

void ram_8bit_in_data4(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_1d(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_data5(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_2d(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_data6(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_3d(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_data7(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_4d(dest->ls189_hi, valptr, timestamp);
}


void (*ram_8bit_in_dataN[])(void *dest, int *valptr, int timestamp) = {

        (void*)ram_8bit_in_data0,
        (void*)ram_8bit_in_data1,
        (void*)ram_8bit_in_data2,
        (void*)ram_8bit_in_data3,
        (void*)ram_8bit_in_data4,
        (void*)ram_8bit_in_data5,
        (void*)ram_8bit_in_data6,
        (void*)ram_8bit_in_data7,
};




void ram_8bit_in_addr0(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_a0(dest->ls189_lo, valptr, timestamp);
    ls189_in_a0(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_addr1(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_a1(dest->ls189_lo, valptr, timestamp);
    ls189_in_a1(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_addr2(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_a2(dest->ls189_lo, valptr, timestamp);
    ls189_in_a2(dest->ls189_hi, valptr, timestamp);
}

void ram_8bit_in_addr3(ram_8bit *dest, int *valptr, int timestamp){

    ls189_in_a3(dest->ls189_lo, valptr, timestamp);
    ls189_in_a3(dest->ls189_hi, valptr, timestamp);
}

void (*ram_8bit_in_addrN[])(void *dest, int *valptr, int timestamp) = {

        (void*)ram_8bit_in_addr0,
        (void*)ram_8bit_in_addr1,
        (void*)ram_8bit_in_addr2,
        (void*)ram_8bit_in_addr3,
};



////////////////////////////////////////////////////////////////////////////////
board_object *ram_8bit_board_create(ram_8bit *reg, int key, char *name){

    board_object *board = board_create(40, 4, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 8; i++){

        j = 7-i;

        sprintf(s,"D%d",i);
        board_add_led(board, reg->led[i],1+4*j,1,s);
    }

    board_add_led(board, reg->ledclk,35,1,"CLK");

    return board;
}
