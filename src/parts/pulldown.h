////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef PULLDOWN_H_
#define PULLDOWN_H_

#include "update.h"

void pulldown_connect(void *dest, event_function_t dest_event_handler);

#endif /* PULLDOWN_H_ */
