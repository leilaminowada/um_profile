/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>

const unsigned WIDTH = 8;
const unsigned WORD_LENGTH = 32;

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;
typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc) 
{
        /* create word and or in 3, 3 bit registers into lowest bits*/
        Um_instruction word = 0;
        word = word | op;
        word = word << 22;
        word = word | ra;
        word = word << 3;
        word = word | rb;
        word = word << 3;
        word = word | rc;

        return word;
}
Um_instruction loadval(unsigned ra, unsigned val)
{
        /* make sure val will fit in 25 bits */
        uint32_t largest = 0;
        largest = ~largest;
        largest = largest >> 7;
        assert(val <= largest);

        /* bitpack val, op code, and register in big endian order */
        Um_instruction word = 0;
        word = word | LV;
        word = word << 3;
        word = word | ra;
        word = word << 25;
        word = word | val;
        return word;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mul(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

Um_instruction output(Um_register c)
{
        /* print what is in register c to output immediately */
        return three_register(OUT, 0, 0, c);      
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

/* helper function to get 1 byte from an int */
char get_byte(uint32_t word, unsigned lsb) 
{
        /* width is always 4 bits and 32 bit int */
        assert((WIDTH + lsb) <= 32);

        /* compute powers of 2 with shifts */
        uint32_t base = 1;
        uint32_t exponent_1 = WIDTH + lsb;
        uint32_t exponent_2 = lsb;
        uint32_t shift_1 = base << exponent_1;
        uint32_t shift_2 = base << exponent_2;

        uint32_t mask = shift_1 - shift_2;

        /* 
        * highest power of 2 computer can compute is 2^31. if 2^32 is
        * computed, the computer produces 1. therefore, if width is 32, 
        * mask should be all ones even though computer fails in the
        * computation above 
        */
        if (WIDTH == WORD_LENGTH)
        {
                mask = 0;
                mask = ~mask;
        }

        mask = mask & word;
        mask = mask >> lsb;
        return mask;
}

void Um_write_sequence(FILE *output, Seq_T stream)
{
        /* write the sequence of um instructions in big endian
        * order to a disk */
        int length = Seq_length(stream);
        for(int i = 0; i < length; i++)
        {
                uint32_t instruction = (uintptr_t)Seq_remlo(stream);
                fputc(get_byte(instruction, 24), output);
                fputc(get_byte(instruction, 16), output);
                fputc(get_byte(instruction, 8), output);
                fputc(get_byte(instruction, 0), output);
        }
}

/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_cmov1_test(Seq_T stream) 
{
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 0));
        emit(stream, loadval(r3, 2));
        emit(stream, three_register(CMOV, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_cmov2_test(Seq_T stream) 
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 3));
        emit(stream, loadval(r3, 5));
        emit(stream, three_register(CMOV, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_activate_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, three_register(ACTIVATE, r3, r1, r2));
        emit(stream, halt());
}

void emit_sload1_test(Seq_T stream) 
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 5));
        emit(stream, three_register(ACTIVATE, r3, r1, r2));
        emit(stream, loadval(r3, 1));
        emit(stream, loadval(r4, 0));
        emit(stream, loadval(r5, 10));
        emit(stream, three_register(SLOAD, r5, r3, r4));
        emit(stream, output(r5));
        emit(stream, halt());
}

void emit_sstore1_test(Seq_T stream) 
{
        /* map segment 1 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 5));
        emit(stream, three_register(ACTIVATE, r3, r1, r2));

        /* segmented store */
        emit(stream, loadval(r3, 10));
        emit(stream, loadval(r4, 0));
        emit(stream, loadval(r5, 1));
        emit(stream, three_register(SSTORE, r5, r4, r3));

        /* segmented load to check */
        emit(stream, loadval(r6, 0));
        emit(stream, three_register(SLOAD, r6, r5, r4));
        emit(stream, output(r6));
        emit(stream, halt());
}

void emit_add1_test(Seq_T stream)
{
        uint32_t most = 0;
        most = ~most;
        most = most >> 7;
        emit(stream, loadval(r1, most));
        emit(stream, loadval(r2, most));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_mul_test(Seq_T stream)
{
        emit(stream, loadval(r1, 5));
        emit(stream, loadval(r2, 7));
        emit(stream, three_register(MUL, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_div_test(Seq_T stream)
{
        emit(stream, loadval(r1, 100));
        emit(stream, loadval(r2, 5));
        emit(stream, three_register(DIV, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}


void emit_nand_test(Seq_T stream)
{
        uint32_t most = 0;
        most = ~most;
        most = most >> 7;
        emit(stream, loadval(r1, most));
        emit(stream, loadval(r2, most));
        emit(stream, three_register(NAND, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_inactivate_test(Seq_T stream)
{
        /* first map second segment */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 5));
        emit(stream, three_register(ACTIVATE, r3, r1, r2));

        /* then unmap it */
        emit(stream, three_register(INACTIVATE, r3, r2, r1));
        emit(stream, halt());
}

void emit_in_test(Seq_T stream)
{
        emit(stream, loadval(r1, 5));
        emit(stream, three_register(IN, r3, r2, r1));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_out_test(Seq_T stream)
{
        emit(stream, loadval(r1, 5));
        emit(stream, three_register(OUT, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}






















