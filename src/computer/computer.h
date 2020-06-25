////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_COMPUTER_COMPUTER_H_
#define SRC_COMPUTER_COMPUTER_H_

#include "reg_8bit.h"
#include "alu_8bit.h"
#include "ram_8bit.h"
#include "progctr.h"
#include "bitconst.h"
#include "ctrunit.h"
#include "reg_out.h"
#include "board.h"
#include "indicator.h"
#include "clkgen.h"
#include "pulldown.h"

typedef struct {

    board_ctx_t *ctx;
    board_object *mainboard;
    event_context_t *ec;
    clkgen *mainclk;
    reg_8bit *regA;
    reg_8bit *regB;
    reg_8bit *regIN;
    board_object *regA_board;
    board_object *regB_board;
    board_object *regIN_board;
    alu_8bit *alu;
    board_object *alu_board;
    ram_8bit *ram;
    board_object *ram_board;
    ctrunit *ctru;
    board_object *ctru_board;
    board_object *ctru_flags_board;
    progctr *pctr;
    board_object *pctr_board;
    reg_out *regout;
    board_object *regout_board;
    indicator *ledbus[8];
    board_object *bus_board;

} computer_t;


void computer_sim();



#endif /* SRC_COMPUTER_COMPUTER_H_ */
