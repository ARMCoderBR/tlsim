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
#include <unistd.h>

#include "ram_8bit.h"
#include "bitconst.h"

////////////////////////////////////////////////////////////////////////////////
void *difpulse_function(void *args){

    fd_set readfds;
    struct timeval tv;
    char buf[16];

    ram_8bit *o = (ram_8bit *)args;

    pipe(o->pipefd);

    o->running = 1;

    for (;o->running;){

        tv.tv_sec = 10;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(o->pipefd[0],&readfds);

        select(1+o->pipefd[0],&readfds,NULL,NULL,&tv);

        if (FD_ISSET(o->pipefd[0],&readfds)){

            read(o->pipefd[0],buf,1);
        }

        if (o->reqpulse){

            board_mutex_lock();
            event_process();
            board_mutex_unlock();
            o->reqpulse = 0;
            o->valpulse = 0;
            ls00_in_b1(o->ls00_clk, &o->valpulse, 0);
        }
    }

    close(o->pipefd[0]);
    close(o->pipefd[1]);

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_reqpulse(ram_8bit *dest){

    char buf = '1';

    write(dest->pipefd[1],&buf,1);
}

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
    ram->ls04_hi = ls04_create();
    ram->ls04_lo = ls04_create();

    int i;
    for (i = 0; i < 8; i++){

        strncpy(lshi,name,sizeof(lshi));
        sprintf(lslo,"-D%d",i);
        strncat(lshi,lslo,sizeof(lshi)/2);
        ram->leddata[i] = indicator_create(lshi);
    }

    ls189_connect_1q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a1);
    ls189_connect_2q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a2);
    ls189_connect_3q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a3);
    ls189_connect_4q(ram->ls189_lo, ram->ls04_lo, (void*)&ls04_in_a4);

    ls04_connect_y1(ram->ls04_lo, ram->leddata[0], (void*)&indicator_in_d0);
    ls04_connect_y2(ram->ls04_lo, ram->leddata[1], (void*)&indicator_in_d0);
    ls04_connect_y3(ram->ls04_lo, ram->leddata[2], (void*)&indicator_in_d0);
    ls04_connect_y4(ram->ls04_lo, ram->leddata[3], (void*)&indicator_in_d0);

    ls189_connect_1q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a1);
    ls189_connect_2q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a2);
    ls189_connect_3q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a3);
    ls189_connect_4q(ram->ls189_hi, ram->ls04_hi, (void*)&ls04_in_a4);

    ls04_connect_y1(ram->ls04_hi, ram->leddata[4], (void*)&indicator_in_d0);
    ls04_connect_y2(ram->ls04_hi, ram->leddata[5], (void*)&indicator_in_d0);
    ls04_connect_y3(ram->ls04_hi, ram->leddata[6], (void*)&indicator_in_d0);
    ls04_connect_y4(ram->ls04_hi, ram->leddata[7], (void*)&indicator_in_d0);

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

    /////////////////////////

    ram->ls157_addr = ls157_create();
    ram->ls173_addreg = ls173_create("ADDREG");
    ram->ls157_datalo = ls157_create();
    ram->ls157_datahi = ls157_create();
    ram->ls157_write = ls157_create();
    ram->ls00_clk = ls00_create();

    ls00_connect_y1(ram->ls00_clk, ram->ls157_write, (void*)&ls157_in_b1);

    ram->ledaddr[0] = indicator_create("A0");
    ram->ledaddr[1] = indicator_create("A1");
    ram->ledaddr[2] = indicator_create("A2");
    ram->ledaddr[3] = indicator_create("A3");
    ram->prog_run = bitswitch_create("ProgRun");
    ram->progaddr[0] = bitswitch_create("RA0");
    ram->progaddr[1] = bitswitch_create("RA1");
    ram->progaddr[2] = bitswitch_create("RA2");
    ram->progaddr[3] = bitswitch_create("RA3");
    ram->progdata[0] = bitswitch_create("RD0");
    ram->progdata[1] = bitswitch_create("RD1");
    ram->progdata[2] = bitswitch_create("RD2");
    ram->progdata[3] = bitswitch_create("RD3");
    ram->progdata[4] = bitswitch_create("RD4");
    ram->progdata[5] = bitswitch_create("RD5");
    ram->progdata[6] = bitswitch_create("RD6");
    ram->progdata[7] = bitswitch_create("RD7");
    ram->progwrite = bitswitch_create("PRW");

    bitswitch_setval(ram->progwrite, 1);

    bitswitch_connect_out(ram->prog_run,ram->ls157_addr,(void*)&ls157_in_sel);
    bitswitch_connect_out(ram->prog_run,ram->ls157_datalo,(void*)&ls157_in_sel);
    bitswitch_connect_out(ram->prog_run,ram->ls157_datahi,(void*)&ls157_in_sel);
    bitswitch_connect_out(ram->prog_run,ram->ls157_write,(void*)&ls157_in_sel);

    bitconst_connect_zero(ram->ls157_addr,(void*)&ls157_in_g);
    bitconst_connect_zero(ram->ls157_datalo,(void*)&ls157_in_g);
    bitconst_connect_zero(ram->ls157_datahi,(void*)&ls157_in_g);
    bitconst_connect_zero(ram->ls157_write,(void*)&ls157_in_g);

    bitswitch_connect_out(ram->progaddr[0],ram->ls157_addr,(void*)&ls157_in_a1);
    bitswitch_connect_out(ram->progaddr[1],ram->ls157_addr,(void*)&ls157_in_a2);
    bitswitch_connect_out(ram->progaddr[2],ram->ls157_addr,(void*)&ls157_in_a3);
    bitswitch_connect_out(ram->progaddr[3],ram->ls157_addr,(void*)&ls157_in_a4);

    ls157_connect_y1(ram->ls157_addr,ram->ls189_lo, (void*)&ls189_in_a0);
    ls157_connect_y2(ram->ls157_addr,ram->ls189_lo, (void*)&ls189_in_a1);
    ls157_connect_y3(ram->ls157_addr,ram->ls189_lo, (void*)&ls189_in_a2);
    ls157_connect_y4(ram->ls157_addr,ram->ls189_lo, (void*)&ls189_in_a3);
    ls157_connect_y1(ram->ls157_addr,ram->ls189_hi, (void*)&ls189_in_a0);
    ls157_connect_y2(ram->ls157_addr,ram->ls189_hi, (void*)&ls189_in_a1);
    ls157_connect_y3(ram->ls157_addr,ram->ls189_hi, (void*)&ls189_in_a2);
    ls157_connect_y4(ram->ls157_addr,ram->ls189_hi, (void*)&ls189_in_a3);
    ls157_connect_y1(ram->ls157_addr,ram->ledaddr[0], (void*)&indicator_in_d0);
    ls157_connect_y2(ram->ls157_addr,ram->ledaddr[1], (void*)&indicator_in_d0);
    ls157_connect_y3(ram->ls157_addr,ram->ledaddr[2], (void*)&indicator_in_d0);
    ls157_connect_y4(ram->ls157_addr,ram->ledaddr[3], (void*)&indicator_in_d0);

    bitswitch_connect_out(ram->progdata[0],ram->ls157_datalo,(void*)&ls157_in_a1);
    bitswitch_connect_out(ram->progdata[1],ram->ls157_datalo,(void*)&ls157_in_a2);
    bitswitch_connect_out(ram->progdata[2],ram->ls157_datalo,(void*)&ls157_in_a3);
    bitswitch_connect_out(ram->progdata[3],ram->ls157_datalo,(void*)&ls157_in_a4);
    bitswitch_connect_out(ram->progdata[4],ram->ls157_datahi,(void*)&ls157_in_a1);
    bitswitch_connect_out(ram->progdata[5],ram->ls157_datahi,(void*)&ls157_in_a2);
    bitswitch_connect_out(ram->progdata[6],ram->ls157_datahi,(void*)&ls157_in_a3);
    bitswitch_connect_out(ram->progdata[7],ram->ls157_datahi,(void*)&ls157_in_a4);

    ls157_connect_y1(ram->ls157_datalo,ram->ls189_lo, (void*)&ls189_in_1d);
    ls157_connect_y2(ram->ls157_datalo,ram->ls189_lo, (void*)&ls189_in_2d);
    ls157_connect_y3(ram->ls157_datalo,ram->ls189_lo, (void*)&ls189_in_3d);
    ls157_connect_y4(ram->ls157_datalo,ram->ls189_lo, (void*)&ls189_in_4d);

    ls157_connect_y1(ram->ls157_datahi,ram->ls189_hi, (void*)&ls189_in_1d);
    ls157_connect_y2(ram->ls157_datahi,ram->ls189_hi, (void*)&ls189_in_2d);
    ls157_connect_y3(ram->ls157_datahi,ram->ls189_hi, (void*)&ls189_in_3d);
    ls157_connect_y4(ram->ls157_datahi,ram->ls189_hi, (void*)&ls189_in_4d);

    bitswitch_connect_out(ram->progwrite,ram->ls157_write,(void*)&ls157_in_a1);
    ls157_connect_y1(ram->ls157_write,ram->ls189_lo, (void*)&ls189_in_we);
    ls157_connect_y1(ram->ls157_write,ram->ls189_hi, (void*)&ls189_in_we);

    ls173_connect_1q(ram->ls173_addreg,ram->ls157_addr,(void*)&ls157_in_b1);
    ls173_connect_2q(ram->ls173_addreg,ram->ls157_addr,(void*)&ls157_in_b2);
    ls173_connect_3q(ram->ls173_addreg,ram->ls157_addr,(void*)&ls157_in_b3);
    ls173_connect_4q(ram->ls173_addreg,ram->ls157_addr,(void*)&ls157_in_b4);

    bitconst_connect_zero(ram->ls173_addreg,(void*)&ls173_in_m);
    bitconst_connect_zero(ram->ls173_addreg,(void*)&ls173_in_n);
    bitconst_connect_zero(ram->ls173_addreg,(void*)&ls173_in_g1);
    bitconst_connect_zero(ram->ls173_addreg,(void*)&ls173_in_clr);

    ram->ledprog = indicator_create("Ledprog");
    ram->ledrun = indicator_create("Ledrun");
    bitswitch_connect_out(ram->prog_run,ram->ledrun,(void*)&indicator_in_d0);
    bitswitch_connect_out(ram->prog_run, ram->ls04_lo, (void*)&ls04_in_a6);
    ls04_connect_y6(ram->ls04_lo, ram->ledprog, (void*)&indicator_in_d0);

    if (name)
        strncpy(ram->name,name,sizeof(ram->name));
    else
        ram->name[0] = 0;

    ram->clk = 2;
    ram->oldclk = 1;
    ram->clk_rootptr = NULL;

    pthread_create(&ram->difpulse_thread, NULL, difpulse_function, ram);

    ram->destroy = (void*)ram_8bit_destroy;

    return ram;
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_destroy (ram_8bit **dest){

    if (dest == NULL) return;
    ram_8bit *b = *dest;
    if (b == NULL) return;

    b->running = 0;
    pthread_join(b->difpulse_thread, NULL);

    DESTROY(b->ls189_hi);
    DESTROY(b->ls189_lo);
    DESTROY(b->ls245_1);
    DESTROY(b->ls04_hi);
    DESTROY(b->ls04_lo);

    int i;
    for (i = 0; i < 4; i++)
        DESTROY(b->ledaddr[i]);

    DESTROY(b->prog_run);

    for (i = 0; i < 4; i++)
        DESTROY(b->progaddr[i]);

    for (i = 0; i < 8; i++)
        DESTROY(b->progdata[i]);

    DESTROY(b->progwrite);

    DESTROY(b->ledprog);
    DESTROY(b->ledrun);

    vallist_destroy((void*)&b->clk_rootptr);

    free(b);
    *dest = NULL;
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
        case 0: connect_fn(from,dest->ls157_datalo,(void*)&ls157_in_b1); break;
        case 1: connect_fn(from,dest->ls157_datalo,(void*)&ls157_in_b2); break;
        case 2: connect_fn(from,dest->ls157_datalo,(void*)&ls157_in_b3); break;
        case 3: connect_fn(from,dest->ls157_datalo,(void*)&ls157_in_b4); break;
        case 4: connect_fn(from,dest->ls157_datahi,(void*)&ls157_in_b1); break;
        case 5: connect_fn(from,dest->ls157_datahi,(void*)&ls157_in_b2); break;
        case 6: connect_fn(from,dest->ls157_datahi,(void*)&ls157_in_b3); break;
        case 7: connect_fn(from,dest->ls157_datahi,(void*)&ls157_in_b4); break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_data0(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b1(dest->ls157_datalo, valptr, timestamp);
    ls173_in_1d(dest->ls173_addreg, valptr, timestamp);
}

void ram_8bit_in_data1(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b2(dest->ls157_datalo, valptr, timestamp);
    ls173_in_2d(dest->ls173_addreg, valptr, timestamp);
}

void ram_8bit_in_data2(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b3(dest->ls157_datalo, valptr, timestamp);
    ls173_in_3d(dest->ls173_addreg, valptr, timestamp);
}

void ram_8bit_in_data3(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b4(dest->ls157_datalo, valptr, timestamp);
    ls173_in_4d(dest->ls173_addreg, valptr, timestamp);
}

void ram_8bit_in_data4(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b1(dest->ls157_datahi, valptr, timestamp);
}

void ram_8bit_in_data5(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b2(dest->ls157_datahi, valptr, timestamp);
}

void ram_8bit_in_data6(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b3(dest->ls157_datahi, valptr, timestamp);
}

void ram_8bit_in_data7(ram_8bit *dest, int *valptr, int timestamp){

    ls157_in_b4(dest->ls157_datahi, valptr, timestamp);
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

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_wdata(ram_8bit *dest, int *valptr, int timestamp){

	ls00_in_a1(dest->ls00_clk, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_waddr(ram_8bit *dest, int *valptr, int timestamp){

	ls173_in_g2(dest->ls173_addreg, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_oe(ram_8bit *dest, int *valptr, int timestamp){

	ls245_in_oe(dest->ls245_1, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ram_8bit_in_clk(ram_8bit *dest, int *valptr, int timestamp){

    ls173_in_clk(dest->ls173_addreg, valptr, timestamp);

#define DIFPULSE 1

#if DIFPULSE

    int val = update_val_multi(&dest->clk_rootptr, valptr);
    if (val > 1) val = 1;

    if (val == dest->oldclk) return;

    dest->oldclk = val;

    if (val){

    	dest->valpulse = 1;
    	ls00_in_b1(dest->ls00_clk, &dest->valpulse, timestamp+10);
        dest->reqpulse = 1;
        ram_8bit_reqpulse(dest);
    }
#else
	ls00_in_b1(dest->ls00_clk, valptr, timestamp);
#endif
}

////////////////////////////////////////////////////////////////////////////////
board_object *ram_8bit_board_create(ram_8bit *ram, int key, char *name){

    board_object *board = board_create(58, 13, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < 8; i++){

        j = 7-i;

        sprintf(s,"D%d",i);
        board_add_led(board, ram->leddata[i],21+4*j,1,s, LED_RED);

        board_add_manual_switch(board, ram->progdata[i], 1+7*j, 7, '0'+i, s);
    }

    char ka[]="hjkl";

    for (i = 0; i < 4; i++){

        j = 3-i;

        sprintf(s,"A%d",i);
        board_add_led(board, ram->ledaddr[i],37+4*j,4,s, LED_YELLOW);
        board_add_manual_switch(board, ram->progaddr[i], 1+7*j, 10, ka[j], s);
    }


    board_add_manual_switch(board, ram->prog_run, 1, 4, 'p',"Pr/Run");
    board_add_manual_switch(board, ram->progwrite, 48, 10, 'w',"Write");

    board_add_led(board, ram->ledrun,1,1,"RUN", LED_GREEN);
    board_add_led(board, ram->ledprog,5,1,"PRG", LED_RED);

    return board;
}
