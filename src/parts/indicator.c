////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#include "update.h"
#include "indicator.h"
#include "board.h"

#include <malloc.h>
#include <string.h>

static void indicator_up(indicator *o){

    if (o->oldvalue != o->value){

        o->oldvalue = o->value;
        if (o->refreshable)
            board_set_refresh();
    }
}

////////////////////////////////////////////////////////////////////////////////
indicator *indicator_create(char *name){

    indicator *o = malloc(sizeof(indicator));

    if (o == NULL)
        return NULL;

    if (name)
        strncpy(o->name, name, sizeof(o->name));
    else
        o->name[0] = 0;

    o->value = 0;
    o->ind0_rootptr = NULL;
    o->ind1_rootptr = NULL;
    o->ind2_rootptr = NULL;
    o->ind3_rootptr = NULL;
    o->oldvalue = 0;
    o->refreshable = 0;

    o->destroy = (void*)indicator_destroy;

    return o;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_destroy(indicator **dest){

	if (dest == NULL) return;
	indicator *b = *dest;
	if (b == NULL) return;

    vallist_destroy(&b->ind0_rootptr);
    vallist_destroy(&b->ind1_rootptr);
    vallist_destroy(&b->ind2_rootptr);
    vallist_destroy(&b->ind3_rootptr);

	free(b);
	*dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d0(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp){

#ifdef DEBUG
    printf("output_update_d0 val:%d\n",val);
#endif

    bitvalue_t val = update_val_multi(&dest->ind0_rootptr, valptr);

    ///logger(???->ec, "indicator_in_d0 [%s] *valptr:%d val:%d TS:%d", dest->name,*valptr,val,timestamp);

    if (val)
        dest->value |= 1;
    else
        dest->value &= ~1;

    indicator_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d1(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp){

#ifdef DEBUG
    printf("output_update_d1 val:%d\n",val);
#endif

    bitvalue_t val = update_val_multi(&dest->ind1_rootptr, valptr);

    if (val)
        dest->value |= 2;
    else
        dest->value &= ~2;

    indicator_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d2(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif

    bitvalue_t val = update_val_multi(&dest->ind2_rootptr, valptr);

    if (val)
        dest->value |= 4;
    else
        dest->value &= ~4;

    indicator_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
void indicator_in_d3(indicator *dest, bitvalue_t *valptr, timevalue_t timestamp){

#ifdef DEBUG
    printf("output_update_d2 val:%d\n",val);
#endif

    bitvalue_t val = update_val_multi(&dest->ind3_rootptr, valptr);

    if (val)
        dest->value |= 8;
    else
        dest->value &= ~8;

    indicator_up(dest);
}

////////////////////////////////////////////////////////////////////////////////
const char dighex[]="0123456789ABCDEF";
void indicator_print(indicator *o){

    if (o->name)
        printf("%s:%c",o->name,dighex[o->value]);
    else
        printf("%c",dighex[o->value]);
}

////////////////////////////////////////////////////////////////////////////////
void indicator_println(indicator *o){

    indicator_print(o);
    printf("\r\n");
}

////////////////////////////////////////////////////////////////////////////////
void indicator_prints(indicator *o){

    indicator_print(o);
    printf(" ");
}
