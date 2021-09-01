////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

// Based on Ben Eater's project (https://www.youtube.com/watch?v=AwUirxi9eBg)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "ctrunit.h"
#include "bitconst.h"

char labels[16][4] = {"FI", " J", "CO", "CE", "OI", "BI", "SU", "SO", "AO", "AI", "II", "IO", "RO", "RI", "MI", "HLT" };

////////////////////////////////////////////////////////////////////////////////
void microcode_initialize_buf(uint8_t *buf){    //Beware: buffer must have at least 2048 bytes!

    // This code initializes the microcode to achieve exactly the same result
    // as Ben's method, but IMO it's much simpler.
    memset(buf,0xff,2048);

    int i;
    int ofs;

    // Microcode Data: https://www.youtube.com/watch?v=Zg1NdPKoosU

    // EEP HIGH Data
    //  HL MI RI RO IO II AI AO
    //  7  6  5  4  3  2  1  0
    int zf, cf;     //cf = A8  zf=a9

    for (zf = 0; zf < 2; zf++)  // Iterate through the ZF values
    for (cf = 0; cf < 2; cf++)  // Iterate through the CF values
    for (i = 0; i < 16; i++){   // All instructions from 0 to 15.

        ofs = (512*zf)+(256*cf)+8*i;    // ZF maps to A9, multiply by 512; CF maps to A8, multiply by 256
        memset(buf+ofs,0,8);            // Fill all steps with 0.

        //FETCH
        buf[0+ofs] = HMI;
        buf[1+ofs] = HRO|HII;

        switch(i){

        case 0x01:  //LDA
            buf[2+ofs] = HIO|HMI;
            buf[3+ofs] = HRO|HAI;
            break;
        case 0x02:  //ADD
            buf[2+ofs] = HIO|HMI;
            buf[3+ofs] = HRO;
            buf[4+ofs] = HAI;
            break;
        case 0x03:  //SUB
            buf[2+ofs] = HIO|HMI;
            buf[3+ofs] = HRO;
            buf[4+ofs] = HAI;
            break;
        case 0x04:  //STA
            buf[2+ofs] = HIO|HMI;
            buf[3+ofs] = HAO|HRI;
            break;
        case 0x05:  //LDI
            buf[2+ofs] = HIO|HAI;
            break;
        case 0x06:  //JMP
            buf[2+ofs] = HIO;
            break;
        case 0x07:  //JC
            if (cf)
                buf[2+ofs] = HIO;
            break;
        case 0x08:  //JZ
            if (zf)
                buf[2+ofs] = HIO;
            break;
        case 0x0e:  //OUT
            buf[2+ofs] = HAO;
            break;
        case 0x0f:  //HLT
            buf[2+ofs] = HHLT;
            break;
        }
    }

    // EEP LOW Data (+offset 128 because A7 is set to one)
    //  SO SU BI OI CE CO J  --
    //  7  6  5  4  3  2  1  0
    for (zf = 0; zf < 2; zf++)  // Iterate through the ZF values
    for (cf = 0; cf < 2; cf++)  // Iterate through the CF values
    for (i = 0; i < 16; i++){   // All instructions from 0 to 15.

        ofs = 128+(512*zf)+(256*cf)+8*i;    // ZF maps to A9, multiply by 512; CF maps to A8, multiply by 256; A7 is forced '1', so adds 128 to the offset.
        memset(buf+ofs,0,8);            // Fill all steps with 0.

        //FETCH
        buf[0+ofs] = LCO;
        buf[1+ofs] = LCE;

        switch(i){
        case 0x01:  //LDA
            break;
        case 0x02:  //ADD
            buf[3+ofs] = LBI;
            buf[4+ofs] = LSO|LFI;
            break;
        case 0x03:  //SUB
            buf[3+ofs] = LBI;
            buf[4+ofs] = LSO|LSU|LFI;
            break;
        case 0x04:  //STA
            break;
        case 0x05:  //LDI
            break;
        case 0x06:  //JMP
            buf[2+ofs] = LJ;
            break;
        case 0x07:  //JC
            if (cf)
                buf[2+ofs] = LJ;
            break;
        case 0x08:  //JZ
            if (zf)
                buf[2+ofs] = LJ;
            break;
        case 0x0e:  //OUT
            buf[2+ofs] = LOI;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
ctrunit *ctrunit_create(event_context_t *ec, char *name){

    ctrunit *ctru = malloc (sizeof(ctrunit));
    if (!ctru) return ctru;

    ctru->ec = ec;

    ctru->ls00 = ls00_create(ec);
    ctru->ls04_1 = ls04_create(ec);
    ctru->ls04_2 = ls04_create(ec);
    ctru->reset_sw = bitswitch_create(ec, "Reset");

    bitswitch_setval(ctru->reset_sw,0);

    bitswitch_connect_out(ctru->reset_sw, ctru->ls00, (void*)&ls00_in_a1);
    bitswitch_connect_out(ctru->reset_sw, ctru->ls00, (void*)&ls00_in_b1);
    ls00_connect_y1(ctru->ls00, ctru->ls00, (void*)&ls00_in_a2);
    ls00_connect_y1(ctru->ls00, ctru->ls00, (void*)&ls00_in_b2);

    int i;
    for (i = 0; i < NSIGNALS_CTRU; i++){

        ctru->led[i] = indicator_create(ec, labels[i]);
        ctru->in_rootptr[i] = NULL; ctru->out_event_handler_root[i] = NULL;
    }

    //// LS161
    ctru->ls161 = ls161_create(ec);
    ctru->ct[0] = indicator_create(ec, "C0");
    ctru->ct[1] = indicator_create(ec, "C1");
    ctru->ct[2] = indicator_create(ec, "C2");
    ls161_connect_qa(ctru->ls161, ctru->ct[0], (void*)&indicator_in_d0);
    ls161_connect_qb(ctru->ls161, ctru->ct[1], (void*)&indicator_in_d0);
    ls161_connect_qc(ctru->ls161, ctru->ct[2], (void*)&indicator_in_d0);

    bitconst_connect_one(ctru->ls161, (void*)&ls161_in_enp);
    bitconst_connect_one(ctru->ls161, (void*)&ls161_in_ent);

    ls00_connect_y1(ctru->ls00, ctru->ls161, (void*)&ls161_in_clear);

    //// LS138
    ctru->ls138 = ls138_create(ec);
    ctru->t[0]  = indicator_create(ec, "T0");
    ctru->t[1]  = indicator_create(ec, "T1");
    ctru->t[2]  = indicator_create(ec, "T2");
    ctru->t[3]  = indicator_create(ec, "T3");
    ctru->t[4]  = indicator_create(ec, "T4");
    ctru->t[5]  = indicator_create(ec, "T5");
    bitconst_connect_zero(ctru->ls138, (void*)&ls138_ing2a);
    bitconst_connect_zero(ctru->ls138, (void*)&ls138_ing2b);
    bitconst_connect_one(ctru->ls138, (void*)&ls138_ing1);
    ls138_connect_y0(ctru->ls138, ctru->t[0], (void*)&indicator_in_d0);
    ls138_connect_y1(ctru->ls138, ctru->t[1], (void*)&indicator_in_d0);
    ls138_connect_y2(ctru->ls138, ctru->t[2], (void*)&indicator_in_d0);
    ls138_connect_y3(ctru->ls138, ctru->t[3], (void*)&indicator_in_d0);
    ls138_connect_y4(ctru->ls138, ctru->t[4], (void*)&indicator_in_d0);
    ls138_connect_y5(ctru->ls138, ctru->t[5], (void*)&indicator_in_d0);

    ls161_connect_qa(ctru->ls161, ctru->ls138, (void*)&ls138_ina);
    ls161_connect_qb(ctru->ls161, ctru->ls138, (void*)&ls138_inb);
    ls161_connect_qc(ctru->ls161, ctru->ls138, (void*)&ls138_inc);

    ctru->ledclk  = indicator_create(ec, "CLK");

    //// EEPROMs

    uint8_t buf[2048];
    microcode_initialize_buf(buf);
    ctru->eep_hi = at28c16_create(ec, "UC-HI",buf);
    ctru->eep_lo = at28c16_create(ec, "UC-LO",buf);

    ls161_connect_qa(ctru->ls161, ctru->eep_hi, (void*)&at28c16_in_a0);
    ls161_connect_qb(ctru->ls161, ctru->eep_hi, (void*)&at28c16_in_a1);
    ls161_connect_qc(ctru->ls161, ctru->eep_hi, (void*)&at28c16_in_a2);
    ls161_connect_qa(ctru->ls161, ctru->eep_lo, (void*)&at28c16_in_a0);
    ls161_connect_qb(ctru->ls161, ctru->eep_lo, (void*)&at28c16_in_a1);
    ls161_connect_qc(ctru->ls161, ctru->eep_lo, (void*)&at28c16_in_a2);

    bitconst_connect_zero(ctru->eep_hi, (void*)&at28c16_in_cs);
    bitconst_connect_zero(ctru->eep_hi, (void*)&at28c16_in_oe);
    bitconst_connect_one(ctru->eep_hi, (void*)&at28c16_in_we);
    bitconst_connect_zero(ctru->eep_lo, (void*)&at28c16_in_cs);
    bitconst_connect_zero(ctru->eep_lo, (void*)&at28c16_in_oe);
    bitconst_connect_one(ctru->eep_lo, (void*)&at28c16_in_we);

    bitconst_connect_zero(ctru->eep_hi, (void*)&at28c16_in_a7);
    bitconst_connect_one(ctru->eep_lo, (void*)&at28c16_in_a7);      // Here A7 is one, so both EEPROMs can be recorded with the same DATA!

    bitconst_connect_zero(ctru->eep_hi, (void*)&at28c16_in_a10);
    bitconst_connect_zero(ctru->eep_lo, (void*)&at28c16_in_a10);

    at28c16_connect_o7(ctru->eep_hi, ctru, (void*)&ctrunit_in_hlt);
    at28c16_connect_o6(ctru->eep_hi, ctru, (void*)&ctrunit_in_mi);
    at28c16_connect_o5(ctru->eep_hi, ctru, (void*)&ctrunit_in_ri);
    at28c16_connect_o4(ctru->eep_hi, ctru, (void*)&ctrunit_in_ro);
    at28c16_connect_o3(ctru->eep_hi, ctru, (void*)&ctrunit_in_io);
    at28c16_connect_o2(ctru->eep_hi, ctru, (void*)&ctrunit_in_ii);
    at28c16_connect_o1(ctru->eep_hi, ctru, (void*)&ctrunit_in_ai);
    at28c16_connect_o0(ctru->eep_hi, ctru, (void*)&ctrunit_in_ao);

    at28c16_connect_o7(ctru->eep_lo, ctru, (void*)&ctrunit_in_so);
    at28c16_connect_o6(ctru->eep_lo, ctru, (void*)&ctrunit_in_su);
    at28c16_connect_o5(ctru->eep_lo, ctru, (void*)&ctrunit_in_bi);
    at28c16_connect_o4(ctru->eep_lo, ctru, (void*)&ctrunit_in_oi);
    at28c16_connect_o3(ctru->eep_lo, ctru, (void*)&ctrunit_in_ce);
    at28c16_connect_o2(ctru->eep_lo, ctru, (void*)&ctrunit_in_co);
    at28c16_connect_o1(ctru->eep_lo, ctru, (void*)&ctrunit_in_j);
    at28c16_connect_o0(ctru->eep_lo, ctru, (void*)&ctrunit_in_fi);

    //// LS173 (Flags)
    ctru->ls173 = ls173_create(ec, "");
    ctru->ledz = indicator_create(ec, "ZF");
    ctru->ledc = indicator_create(ec, "CF");

    bitconst_connect_zero(ctru->ls173, (void*)&ls173_in_m);
    bitconst_connect_zero(ctru->ls173, (void*)&ls173_in_n);
    bitconst_connect_zero(ctru->ls173, (void*)&ls173_in_g1);

    ls04_connect_y6(ctru->ls04_2, ctru->ls173, (void*)&ls173_in_g2);    // Load Strobe from the microcode in EEPROM (O0, low EEPROM - inverted)

    ls173_connect_1q(ctru->ls173, ctru->ledz, (void*)indicator_in_d0);
    ls173_connect_2q(ctru->ls173, ctru->ledc, (void*)indicator_in_d0);

    ls173_connect_2q(ctru->ls173, ctru->eep_hi, (void*)&at28c16_in_a8); // CF mapped in A8
    ls173_connect_1q(ctru->ls173, ctru->eep_hi, (void*)&at28c16_in_a9); // ZF mapped in A9
    ls173_connect_2q(ctru->ls173, ctru->eep_lo, (void*)&at28c16_in_a8); // CF mapped in A8
    ls173_connect_1q(ctru->ls173, ctru->eep_lo, (void*)&at28c16_in_a9); // ZF mapped in A9


    //Clear
    ls00_connect_y2(ctru->ls00, ctru->ls173, (void*)&ls173_in_clr);

    // Reset Tn Counter (Turbo)

    bitconst_connect_zero(ctru->ls161, (void*)&ls161_in_da);
    bitconst_connect_zero(ctru->ls161, (void*)&ls161_in_db);
    bitconst_connect_zero(ctru->ls161, (void*)&ls161_in_dc);
    bitconst_connect_zero(ctru->ls161, (void*)&ls161_in_dd);

    ctru->ls32_pri1 = ls32_create(ec);
    ctru->ls32_pri2 = ls32_create(ec);
    ctru->ls32_sec = ls32_create(ec);
    ctru->ls32_ter = ls32_create(ec);

    at28c16_connect_o0(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_a1);
    at28c16_connect_o1(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_b1);
    at28c16_connect_o2(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_a2);
    at28c16_connect_o3(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_b2);
    at28c16_connect_o4(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_a3);
    at28c16_connect_o5(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_b3);
    at28c16_connect_o6(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_a4);
    at28c16_connect_o7(ctru->eep_lo, ctru->ls32_pri1, (void*)&ls32_in_b4);

    at28c16_connect_o0(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_a1);
    at28c16_connect_o1(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_b1);
    at28c16_connect_o2(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_a2);
    at28c16_connect_o3(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_b2);
    at28c16_connect_o4(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_a3);
    at28c16_connect_o5(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_b3);
    at28c16_connect_o6(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_a4);
    at28c16_connect_o7(ctru->eep_hi, ctru->ls32_pri2, (void*)&ls32_in_b4);

    ls32_connect_y1(ctru->ls32_pri1, ctru->ls32_sec, (void*)ls32_in_a1);
    ls32_connect_y2(ctru->ls32_pri1, ctru->ls32_sec, (void*)ls32_in_b1);
    ls32_connect_y3(ctru->ls32_pri1, ctru->ls32_sec, (void*)ls32_in_a2);
    ls32_connect_y4(ctru->ls32_pri1, ctru->ls32_sec, (void*)ls32_in_b2);

    ls32_connect_y1(ctru->ls32_pri2, ctru->ls32_sec, (void*)ls32_in_a3);
    ls32_connect_y2(ctru->ls32_pri2, ctru->ls32_sec, (void*)ls32_in_b3);
    ls32_connect_y3(ctru->ls32_pri2, ctru->ls32_sec, (void*)ls32_in_a4);
    ls32_connect_y4(ctru->ls32_pri2, ctru->ls32_sec, (void*)ls32_in_b4);

    ls32_connect_y1(ctru->ls32_sec, ctru->ls32_ter, (void*)ls32_in_a1);
    ls32_connect_y2(ctru->ls32_sec, ctru->ls32_ter, (void*)ls32_in_b1);
    ls32_connect_y3(ctru->ls32_sec, ctru->ls32_ter, (void*)ls32_in_a2);
    ls32_connect_y4(ctru->ls32_sec, ctru->ls32_ter, (void*)ls32_in_b2);

    ls32_connect_y1(ctru->ls32_ter, ctru->ls32_ter, (void*)ls32_in_a3);
    ls32_connect_y2(ctru->ls32_ter, ctru->ls32_ter, (void*)ls32_in_b3);
    ls32_connect_y3(ctru->ls32_ter, ctru->ls32_ter, (void*)ls32_in_a4);

    ctru->turbo_sw = bitswitch_create(ec, "Turbo");
    bitswitch_setval(ctru->turbo_sw,1);
    bitswitch_connect_out(ctru->turbo_sw, ctru->ls32_ter, (void*)&ls32_in_b4);
    ls32_connect_y4(ctru->ls32_ter, ctru->ls161, (void*)&ls161_in_load);

    ctru->destroy = (void*)ctrunit_destroy;

    return ctru;
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_destroy (ctrunit **dest){

    if (dest == NULL) return;
    ctrunit *b = *dest;
    if (b == NULL) return;

    DESTROY(b->ls00);
    DESTROY(b->ls04_1);
    DESTROY(b->ls04_2);
    DESTROY(b->reset_sw);

    DESTROY(b->ls161);
    DESTROY(b->ct[0]);
    DESTROY(b->ct[1]);
    DESTROY(b->ct[2]);

    DESTROY(b->ls138);
    DESTROY(b->t[0]);
    DESTROY(b->t[1]);
    DESTROY(b->t[2]);
    DESTROY(b->t[3]);
    DESTROY(b->t[4]);
    DESTROY(b->t[5]);

    int i;
    for (i = 0; i < NSIGNALS_CTRU; i++){

        DESTROY(b->led[i]);
        vallist_destroy(&b->in_rootptr[i]);
        ehandler_destroy(&b->out_event_handler_root[i]);
    }

    DESTROY(b->ls161);
    DESTROY(b->ls138);
    DESTROY(b->ct[0]);
    DESTROY(b->ct[1]);
    DESTROY(b->ct[2]);
    DESTROY(b->t[0]);
    DESTROY(b->t[1]);
    DESTROY(b->t[2]);
    DESTROY(b->t[3]);
    DESTROY(b->t[4]);
    DESTROY(b->t[5]);
    DESTROY(b->ledclk);
    DESTROY(b->eep_hi);
    DESTROY(b->eep_lo);

    DESTROY(b->ls173);
    DESTROY(b->ledz);
    DESTROY(b->ledc);

    DESTROY(b->ls32_pri1);
    DESTROY(b->ls32_pri2);
    DESTROY(b->ls32_sec);
    DESTROY(b->ls32_ter);

    free(b);
    *dest = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in(ctrunit *dest, int index, bitvalue_t *valptr, timevalue_t timestamp){

    indicator_in_d0(dest->led[index], valptr, timestamp);

    bitvalue_t val = update_val_multi(&dest->in_rootptr[index], valptr);

    if (val > 1)
        val = 1;

    dest->val[index] = val;

    if (val == dest->o_val[index]) return;

    dest->o_val[index] = val;

    event e;
    e.event_handler_root = dest->out_event_handler_root[index];
    e.valueptr = &dest->val[index];
    e.timestamp = timestamp+1;
    event_insert(dest->ec, &e);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out(ctrunit *source, int index, void *dest, event_function_t dest_event_handler){

    new_ehandler(&source->out_event_handler_root[index], dest, dest_event_handler);
    dest_event_handler(dest,&source->val[index],0);
}

////////////////////////////////////////////////////////////////////////////////
board_object *ctrunit_board_create(ctrunit *reg, int key, char *name){

    board_object *board = board_create(66, 7, key, name);

    if (!board) return board;

    char s[32];
    int i,j;

    for (i = 0; i < NSIGNALS_CTRU; i++){

        j = NSIGNALS_CTRU-i;
        sprintf(s,labels[i],i);
        board_add_led(board, reg->led[i],1+4*(j-1),1,s, LED_BLUE);
    }

    board_add_manual_switch(board, reg->reset_sw, 1, 4, 'r', "RST");

    board_add_led(board, reg->ct[2],9,4,"C2", LED_RED);
    board_add_led(board, reg->ct[1],13,4,"C1", LED_RED);
    board_add_led(board, reg->ct[0],17,4,"C0", LED_RED);

    board_add_led(board, reg->t[0],25,4,"T0", LED_GREEN);
    board_add_led(board, reg->t[1],29,4,"T1", LED_GREEN);
    board_add_led(board, reg->t[2],33,4,"T2", LED_GREEN);
    board_add_led(board, reg->t[3],37,4,"T3", LED_GREEN);
    board_add_led(board, reg->t[4],41,4,"T4", LED_GREEN);
    board_add_led(board, reg->t[5],45,4,"T5", LED_GREEN);

    board_add_manual_switch(board, reg->turbo_sw, 51, 4, 't', "Turbo");

    board_add_led(board, reg->ledclk,61,4,"CLK", LED_BLUE);

    return board;
}

////////////////////////////////////////////////////////////////////////////////
board_object *ctrunit_board_flags_create(ctrunit *reg, int key, char *name){

    board_object *board = board_create(13, 5, key, name);

    if (!board) return board;

    board_add_led(board, reg->ledc,3,2,"CF", LED_WHITE);
    board_add_led(board, reg->ledz,7,2,"ZF", LED_WHITE);

    return board;
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_hlt(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ctrunit_in(dest, HLT, valptr, timestamp);
    indicator_in_d0(dest->led[HLT], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_hlt(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ctrunit_connect_out(source, HLT, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_mi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a4(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[MI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_mi(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y4(source->ls04_2, dest, dest_event_handler);
    //ctrunit_connect_out(source, MI, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ri(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ctrunit_in(dest, RI, valptr, timestamp);
    indicator_in_d0(dest->led[RI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ri(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ctrunit_connect_out(source, RI, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ro(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a1(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[RO], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ro(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y1(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, RO, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_io(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a2(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[IO], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_io(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y2(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, IO, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ii(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a3(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[II], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ii(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y3(source->ls04_2, dest, dest_event_handler);
    //ctrunit_connect_out(source, II, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ai(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a1(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[AI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ai(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y1(source->ls04_2, dest, dest_event_handler);
    //ctrunit_connect_out(source, AI, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ao(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a3(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[AO], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ao(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y3(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, AO, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_so(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a4(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[SO], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_so(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y4(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, SO, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_su(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ctrunit_in(dest, SU, valptr, timestamp);
    indicator_in_d0(dest->led[SU], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_su(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ctrunit_connect_out(source, SU, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_bi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a2(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[BI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_bi(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y2(source->ls04_2, dest, dest_event_handler);
    //ctrunit_connect_out(source, BI, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_oi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a5(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[OI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_oi(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y5(source->ls04_2, dest, dest_event_handler);
    //ctrunit_connect_out(source, OI, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_ce(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ctrunit_in(dest, CE, valptr, timestamp);
    indicator_in_d0(dest->led[CE], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_ce(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ctrunit_connect_out(source, CE, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_co(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a5(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[CO], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_co(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y5(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, CO, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_j(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a6(dest->ls04_1, valptr, timestamp);
    indicator_in_d0(dest->led[J], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_fi(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls04_in_a6(dest->ls04_2, valptr, timestamp);
    indicator_in_d0(dest->led[FI], valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_clk(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_clk(dest->ls173, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_clkn(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls161_in_clk(dest->ls161, valptr, timestamp);
    indicator_in_d0(dest->ledclk, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_j(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls04_connect_y6(source->ls04_1, dest, dest_event_handler);
    //ctrunit_connect_out(source, J, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_reset(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls00_connect_y2(source->ls00, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_connect_out_nreset(ctrunit *source, void *dest, event_function_t dest_event_handler){

    ls00_connect_y1(source->ls00, dest, dest_event_handler);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_instr0(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    at28c16_in_a3(dest->eep_hi, valptr, timestamp);
    at28c16_in_a3(dest->eep_lo, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_instr1(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    at28c16_in_a4(dest->eep_hi, valptr, timestamp);
    at28c16_in_a4(dest->eep_lo, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_instr2(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    at28c16_in_a5(dest->eep_hi, valptr, timestamp);
    at28c16_in_a5(dest->eep_lo, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_instr3(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    at28c16_in_a6(dest->eep_hi, valptr, timestamp);
    at28c16_in_a6(dest->eep_lo, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_zero(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_1d(dest->ls173, valptr, timestamp);
}

////////////////////////////////////////////////////////////////////////////////
void ctrunit_in_carry(ctrunit *dest, bitvalue_t *valptr, timevalue_t timestamp){

    ls173_in_2d(dest->ls173, valptr, timestamp);
}
