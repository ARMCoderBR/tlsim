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
    int value;
} output;

output *output_create(char *name);

void output_update_d0(output *o, int val, int timestamp);
void output_update_d1(output *o, int val, int timestamp);
void output_update_d2(output *o, int val, int timestamp);
void output_update_d3(output *o, int val, int timestamp);

void output_print(output *o);
void output_println(output *o);
void output_prints(output *o);

#endif /* OUTPUT_H_ */
