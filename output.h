/*
 * output.h
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_


typedef struct  {

    char *name;
    int *getval;
} output;

void output_connect(output *o, char *name, int *getval);

void output_print(output *o);
void output_println(output *o);

#endif /* OUTPUT_H_ */
