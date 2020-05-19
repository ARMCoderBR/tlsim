////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef ALU_8BIT_H_
#define ALU_8BIT_H_

// Based on Ben Eater's project (https://www.youtube.com/watch?v=S-3fXU3FZQc)

#include "ls02.h"
#include "ls08.h"
#include "ls86.h"
#include "ls283.h"
#include "ls245.h"
#include "indicator.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct {

    part_destroy_function_t destroy;
    ls86 *ls86_hi, *ls86_lo;
    ls283 *ls283_hi, *ls283_lo;
    ls245 *ls245_1;
    indicator *led[8];
    indicator *ledz,*ledc;
    char name[30];
    ls08 *ls08;
    ls02 *ls02;
    event_context_t *ec;
} alu_8bit;

alu_8bit *alu_8bit_create(event_context_t *ec, char *name);

void alu_8bit_destroy (alu_8bit **dest);

void alu_8bit_connect_bit_out (alu_8bit *source, int index, void *dest, event_function_t dest_event_handler);

void alu_8bit_in_dataA_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from,alu_8bit *dest,int index);

void alu_8bit_in_dataB_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from,alu_8bit *dest,int index);

void alu_8bit_in_sub_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, alu_8bit *dest);

void alu_8bit_in_enable_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, alu_8bit *dest);

void alu_8bit_connect_carry_out (alu_8bit *source, void *dest, event_function_t dest_event_handler);

void alu_8bit_connect_zero_out (alu_8bit *source, void *dest, event_function_t dest_event_handler);

///////////////////////////////

extern event_function_t alu_8bit_in_dataAN[];
extern event_function_t alu_8bit_in_dataBN[];

board_object *alu_8bit_board_create(alu_8bit *alu, int key, char *name); // Requer NCURSES

#endif /* ALU_8BIT_H_ */
