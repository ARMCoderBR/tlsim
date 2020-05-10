////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_COMPUTER_CLKGEN_H_
#define SRC_COMPUTER_CLKGEN_H_

#include "update.h"

typedef struct{

    part_destroy_function_t destroy;
    bitvalue_t value, valuen;
    ehandler *out_event_handler_root;
    ehandler *outn_event_handler_root;
    pthread_t clkthread;
    bool_t running;
    uint32_t period_us;
    bitvalue_t halt;
    vallist *halt_rootptr;
    bool_t pause;
    bool_t step;
    char name[30];
} clkgen;

clkgen *clkgen_create(char *name, int period_us);

void clkgen_destroy (clkgen **dest);

void clkgen_in_halt(clkgen *dest, bitvalue_t *valptr, timevalue_t timestamp);

void clkgen_connect_out(clkgen *source, void *dest, event_function_t dest_event_handler);
void clkgen_connect_outn(clkgen *source, void *dest, event_function_t dest_event_handler);


void clkgen_pause(clkgen *dest, bool_t status);
void clkgen_step(clkgen *dest);
void clkgen_set_us(clkgen *dest, uint32_t us);


#endif /* SRC_COMPUTER_CLKGEN_H_ */
