////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS189_H_
#define LS189_H_

#include "update.h"

#define NUM_BITS_LS189 4
#define NUM_WORDS_LS189 16

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t inpd[NUM_BITS_LS189];
	vallist *inpd_rootptr[NUM_BITS_LS189];

	bitvalue_t in_addr[4];
    vallist *in_addr_rootptr[4];
    bitvalue_t in_cs, in_we;
    vallist *in_cs_rootptr, *in_we_rootptr;
    uint16_t current_addr;

    bitvalue_t latd[NUM_WORDS_LS189][NUM_BITS_LS189];
    bitvalue_t outq[NUM_BITS_LS189];
    bitvalue_t outq_o[NUM_BITS_LS189];
    ehandler *outq_event_handler_root[NUM_BITS_LS189];
    char name[32];
    event_context_t *ec;
} ls189;

ls189 *ls189_create(event_context_t *ec, char *name);

void ls189_destroy (ls189 **dest);

void ls189_connect_1q(ls189 *source, void *dest, event_function_t dest_event_handler);
void ls189_connect_2q(ls189 *source, void *dest, event_function_t dest_event_handler);
void ls189_connect_3q(ls189 *source, void *dest, event_function_t dest_event_handler);
void ls189_connect_4q(ls189 *source, void *dest, event_function_t dest_event_handler);

void ls189_in_1d(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_2d(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_3d(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_4d(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls189_in_a0(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_a1(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_a2(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_a3(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);

void ls189_in_cs(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls189_in_we(ls189 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS189_H_ */
