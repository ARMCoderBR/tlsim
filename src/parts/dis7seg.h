////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef DIS7SEG_H_
#define DIS7SEG_H_

#include <pthread.h>

#include "update.h"

#define MSK_DP 0x80
#define MSK_A  0x40
#define MSK_B  0x20
#define MSK_C  0x10
#define MSK_D  0x08
#define MSK_E  0x04
#define MSK_F  0x02
#define MSK_G  0x01


typedef enum{

    COMMON_K,
    COMMON_A
} dis7seg_type;

typedef struct  {

    part_destroy_function_t destroy;
    char name[30];
    vallist *seg_rootptr[8];
    bitvalue_t segval[8];
    vallist *common_rootptr;
    bitvalue_t common_val, common_val_old;
    int segmap, segmap_old;
    int presegmap;
    dis7seg_type type;
    bool_t refreshable;
    bool_t running;
    pthread_t persist_thread;
    int count_persist;
    int delay_latch_display;
} dis7seg;

dis7seg *dis7seg_create(dis7seg_type type, char *name);

void dis7seg_destroy(dis7seg **dest);

void dis7seg_in_sega(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segb(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segc(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segd(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_sege(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segf(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segg(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_segdp(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);
void dis7seg_in_common(dis7seg *dest, bitvalue_t *valptr, timevalue_t timestamp);

int map7seg(int val);

#endif /* DIS7SEG_H_ */
