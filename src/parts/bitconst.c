////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>

#include "update.h"
#include "bitconst.h"


bitvalue_t bit_one = 1;
bitvalue_t bit_zero = 0;

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_one(void *dest, event_function_t dest_event_handler){

    dest_event_handler(dest,&bit_one,0);
}

////////////////////////////////////////////////////////////////////////////////
void bitconst_connect_zero(void *dest, event_function_t dest_event_handler){

    dest_event_handler(dest,&bit_zero,0);
}
