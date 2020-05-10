////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef REG_OUT_H_
#define REG_OUT_H_

// Based on Ben Eater's project (https://www.youtube.com/watch?v=CiMaWbz_6E8)

#include "ls173.h"
#include "board.h"
#include "at28c16.h"
#include "clkgen.h"
#include "ls76.h"
#include "ls139.h"
#include "bitswitch.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct {

    part_destroy_function_t destroy;
    ls173 *ls173_hi, *ls173_lo;
    at28c16 *eep1;
    dis7seg *display[4];
    indicator *led[8];
    indicator *ledclk;
    clkgen *clk;
    indicator *ledclki;
    ls76 *ls76;
    indicator *led76_0, *led76_1;
    ls139* ls139;
    bitswitch *sw_signed;
    char name[30];

} reg_out;

reg_out *reg_out_create(char *name);

void reg_out_destroy (reg_out **dest);

void reg_out_in_data_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from,reg_out *dest,int index);

void reg_out_in_load_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from,reg_out *dest);

void reg_out_in_clear_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_out *dest);

void reg_out_in_clock_from(void (*connect_fn)(void *source, void *dest, event_function_t dest_event_handler),
                        void *from, reg_out *dest);

///////////////////////////////
void reg_out_in_clock(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);


extern event_function_t reg_out_in_dataN[];


void reg_out_in_data0(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data1(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data2(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data3(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data4(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data5(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data6(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);
void reg_out_in_data7(reg_out *dest, bitvalue_t *valptr, timevalue_t timestamp);


board_object *reg_out_board_create(reg_out *reg, int key, char *name); // Requer NCURSES

#endif /* REG_OUT_H_ */
