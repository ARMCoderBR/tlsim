/*
 * main.c
 *
 *  Created on: 16 de jul de 2019
 *      Author: milton
 */


#include <stdio.h>
#include <ncurses.h>

#include "board.h"
#include "tests.h"
#include "computer.h"

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){

    //testls191();
    //gatetest();

    computer_sim();

    return 0;
}
