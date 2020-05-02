/*
 * dis7seg.c
 *
 *  Created on: 02 de mai de 2020
 *      Author: milton
 */

#include "update.h"
#include "dis7seg.h"

#include <malloc.h>
#include <string.h>

const int segmasks[] = { MSK_DP, MSK_A, MSK_B, MSK_C, MSK_D, MSK_E, MSK_F, MSK_G };

////////////////////////////////////////////////////////////////////////////////
dis7seg *dis7seg_create(dis7seg_type type, char *name){

    dis7seg *o = malloc(sizeof(dis7seg));

    if (o == NULL)
        return NULL;

    if (name)
        strncpy(o->name, name, sizeof(o->name));
    else
        o->name[0] = 0;

    o->type = type;
    o->segmap = 0;

    int i;
    for (i = 0; i < 8; i++){

        o->seg_rootptr[i] = NULL;
        o->segval[i] = 0;
    }
    o->common_rootptr = NULL;
    o->common_val = 0;

    return o;
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_up(dis7seg *dest){

    int i;
    for (i = 0; i < 8; i++){
        if (dest->segval[i] == 2)
            dest->segmap &= ~segmasks[i];
        else{

            if (dest->segval[i] == 1){

                if (dest->type == COMMON_K)
                    dest->segmap |= segmasks[i];    //Catodo comum
                else
                    dest->segmap &= ~segmasks[i];   //Anodo comum
            }
            else{   // Valor 0

                if (dest->type == COMMON_K)
                    dest->segmap &= ~segmasks[i];  //Catodo comum
                else
                    dest->segmap |= segmasks[i];   //Anodo comum
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segdp(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[0] = update_val_multi(&dest->seg_rootptr[0], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_sega(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[1] = update_val_multi(&dest->seg_rootptr[1], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segb(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[2] = update_val_multi(&dest->seg_rootptr[2], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segc(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[3] = update_val_multi(&dest->seg_rootptr[3], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segd(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[4] = update_val_multi(&dest->seg_rootptr[4], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_sege(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[5] = update_val_multi(&dest->seg_rootptr[5], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segf(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[6] = update_val_multi(&dest->seg_rootptr[6], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_segg(dis7seg *dest, int *valptr, int timestamp){

    dest->segval[7] = update_val_multi(&dest->seg_rootptr[7], valptr);
    dis7seg_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_in_common(dis7seg *dest, int *valptr, int timestamp){

    int val = update_val_multi(&dest->common_rootptr, valptr);

    dest->common_val = 0;

    if ((dest->type == COMMON_K) && (val == 0))
        dest->common_val = 1;

    if ((dest->type == COMMON_A) && (val == 1))
        dest->common_val = 1;
}

int map7seg(int val){

/*
    A
 +-----+
F|     |B
 +--G--+
E|     |C
 +-----+ o
    D
*/
    int aseg[] = {

            0b01111110,
            0b00110000,
            0b01101101,
            0b01111001,
            0b00110011,
            0b01011011,
            0b01011111,
            0b01110000,
            0b01111111,
            0b01111011,
    };

    if (val < 10)
        return aseg[val];
    else
        return 0;
}
