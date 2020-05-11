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
#include "pulldown.h"


bitvalue_t bit_pulldown = 3;

////////////////////////////////////////////////////////////////////////////////
void pulldown_connect(void *dest, event_function_t dest_event_handler){

    dest_event_handler(dest,&bit_pulldown,0);
}

