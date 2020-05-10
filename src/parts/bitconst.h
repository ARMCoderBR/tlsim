////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef BITCONST_H_
#define BITCONST_H_

#include "update.h"

void bitconst_connect_one(void *dest, event_function_t dest_event_handler);
void bitconst_connect_zero(void *dest, event_function_t dest_event_handler);

#endif /* BITSWITCH_H_ */
