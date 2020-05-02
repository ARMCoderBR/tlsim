/*
 * dis7seg.h
 *
 *  Created on: 02 de mai de 2020
 *      Author: milton
 */

#ifndef DIS7SEG_H_
#define DIS7SEG_H_


#define MSK_A  0x80
#define MSK_B  0x40
#define MSK_C  0x20
#define MSK_D  0x10
#define MSK_E  0x08
#define MSK_F  0x04
#define MSK_G  0x02
#define MSK_DP 0x01


typedef enum{

    COMMON_K,
    COMMON_A
} dis7seg_type;

typedef struct  {

    char name[30];
    vallist *seg_rootptr[8];
    int segval[8];
    vallist *common_rootptr;
    int common_val;
    int segmap;
    dis7seg_type type;
} dis7seg;

dis7seg *dis7seg_create(dis7seg_type type, char *name);

void dis7seg_in_sega(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segb(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segc(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segd(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_sege(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segf(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segg(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_segdp(dis7seg *dest, int *valptr, int timestamp);
void dis7seg_in_common(dis7seg *dest, int *valptr, int timestamp);

#endif /* DIS7SEG_H_ */