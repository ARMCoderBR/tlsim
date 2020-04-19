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

    //do_testls191();
    //gatetest();

    logger_init();

    computer_sim();

    logger_end();

    return 0;
}
