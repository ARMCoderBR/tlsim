////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef LS283_H_
#define LS283_H_

#include "update.h"

#define LS283_NBITS 4

typedef struct {

    part_destroy_function_t destroy;
    bitvalue_t ina[LS283_NBITS], inb[LS283_NBITS], cin;
    vallist *ina_rootptr[LS283_NBITS], *inb_rootptr[LS283_NBITS], *cin_rootptr;
    bitvalue_t oldy[LS283_NBITS], oldcout;
    bitvalue_t y[LS283_NBITS], cout;
    ehandler *y_event_handler_root[LS283_NBITS];
    ehandler *cout_event_handler_root;
    char name[30];
} ls283;

ls283 *ls283_create(char *name);

void ls283_destroy(ls283 **dest);

void ls283_connect_y1(ls283 *source, void *dest, event_function_t dest_event_handler);
void ls283_connect_y2(ls283 *source, void *dest, event_function_t dest_event_handler);
void ls283_connect_y3(ls283 *source, void *dest, event_function_t dest_event_handler);
void ls283_connect_y4(ls283 *source, void *dest, event_function_t dest_event_handler);
void ls283_connect_cout(ls283 *source, void *dest, event_function_t dest_event_handler);

void ls283_in_a1(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_a2(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_a3(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_a4(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_b1(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_b2(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_b3(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_b4(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);
void ls283_in_cin(ls283 *dest, bitvalue_t *valptr, timevalue_t timestamp);

#endif /* LS283_H_ */
