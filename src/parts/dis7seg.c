/*
 * dis7seg.c
 *
 *  Created on: 02 de mai de 2020
 *      Author: milton
 */

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "update.h"
#include "dis7seg.h"
#include "board.h"

const int segmasks[] = { MSK_DP, MSK_A, MSK_B, MSK_C, MSK_D, MSK_E, MSK_F, MSK_G };

////////////////////////////////////////////////////////////////////////////////
void *persist_function(void *args){

    dis7seg *o = (dis7seg *)args;

    o->running = 1;

    for (;o->running;){

        usleep(5000);

        if (!o->common_val){

        	o->delay_latch_display = 0;
        }else{
        	o->count_persist = 20;

			if (o->delay_latch_display < 1)
				o->delay_latch_display++;
			else{
					o->segmap = o->presegmap;
					if (o->segmap_old != o->segmap){
					      o->segmap_old = o->segmap;
					      if (o->refreshable)
					          board_set_refresh();
					}
			}
        }

        if (o->count_persist){

            --o->count_persist;

            if (!o->count_persist){
            	if (o->refreshable)
            		board_set_refresh();
            }
        }
    }

    return NULL;
}

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
    o->presegmap = o->segmap = 0;

    int i;
    for (i = 0; i < 8; i++){

        o->seg_rootptr[i] = NULL;
        o->segval[i] = 0;
    }
    o->common_rootptr = NULL;
    o->common_val = o->common_val_old = 0;

    pthread_create(&o->persist_thread, NULL, persist_function, o);
    return o;
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_destroy(dis7seg **dest){

	if (dest == NULL) return;
	dis7seg *b = *dest;
	if (b == NULL) return;

	b->running = 0;
	pthread_join(b->persist_thread, NULL);

	int i;
    for (i = 0; i < 8; i++){

        vallist_destroy(&b->seg_rootptr[i]);
    }

    vallist_destroy(&b->common_rootptr);

	free(b);
	*dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void dis7seg_up(dis7seg *dest){

    int i;
    for (i = 0; i < 8; i++){
        if (dest->segval[i] == 2)
            dest->presegmap &= ~segmasks[i];
        else{

            if (dest->segval[i] == 1){

                if (dest->type == COMMON_K)
                    dest->presegmap |= segmasks[i];    //Catodo comum
                else
                    dest->presegmap &= ~segmasks[i];   //Anodo comum
            }
            else{   // Valor 0

                if (dest->type == COMMON_K)
                    dest->presegmap &= ~segmasks[i];  //Catodo comum
                else
                    dest->presegmap |= segmasks[i];   //Anodo comum
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

    if (dest->common_val_old != dest->common_val){

        dest->common_val_old = dest->common_val;

//        if (dest->refreshable){
//
//            if (dest->common_val){
//                //dest->segmap = dest->presegmap;
//                dest->count_persist = 10;
//                //board_set_refresh();
//            }
//        }
    }
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
