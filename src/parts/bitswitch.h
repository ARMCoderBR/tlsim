////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef BITSWITCH_H_
#define BITSWITCH_H_

#include "update.h"

typedef struct{

    part_destroy_function_t destroy;
    bitvalue_t oldvalue;
    bitvalue_t value;
    ehandler *out_event_handler_root;
    char name[30];
} bitswitch;

bitswitch *bitswitch_create(char *name);

void bitswitch_destroy(bitswitch **dest);

void bitswitch_connect_out(bitswitch *source, void *dest, event_function_t dest_event_handler);

void bitswitch_setval(bitswitch *s, bitvalue_t val);

#endif /* BITSWITCH_H_ */
