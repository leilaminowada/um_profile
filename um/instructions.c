/*
 * instructions.c
 * 
 * This is the .c that contains all 14 of the 
 * instructions for our UM and their implementations
 *
 * SECRET: how to implement each of the 14 UM instructions
 *         and how to refernce a UM register
 *
 * COMP40 HW6
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "seg_mem.h"
#include "write.h"

const int32_t ENDFILE = 256;


/* conditional move, if what's in register c is not 0, 
* then make register a contents = register b, op = 0 */
void cmov(uint32_t reg[], uint32_t ra, uint32_t rb, uint32_t rc)
{
 	if(reg[rc] != 0) {
                reg[ra] = reg[rb];
        }
}

/* segmented load, register a contents = segment b of register c, op = 1 */
void sload(Mem_T memory, uint32_t reg[], uint32_t ra, uint32_t rb, 
        uint32_t rc)
{
        reg[ra] = Mem_load(memory, reg[rb], reg[rc]);
}

/* segmented store, segment a in register b = register c, op = 2 */
void sstore(Mem_T memory, uint32_t reg[], uint32_t ra, uint32_t rb, 
        uint32_t rc)
{
        Mem_store(memory, reg[ra], reg[rb], reg[rc]);
}

/* add b c to a, op = 3 */
void add(uint32_t reg[], uint32_t ra, uint32_t rb, uint32_t rc)
{
        reg[ra] = (reg[rb] + reg[rc]);
}

/* mulitply b c to a, op = 4 */
void mul(uint32_t reg[], uint32_t ra, uint32_t rb, uint32_t rc)
{
        reg[ra] = (reg[rb] * reg[rc]);
}

/* div b by c to a, op = 5 */
void divide(uint32_t reg[], uint32_t ra, uint32_t rb, uint32_t rc)
{
        reg[ra] = reg[rb] / reg[rc];
}

/* opposite of b and c, to a, op = 6 */
void nand(uint32_t reg[], uint32_t ra, uint32_t rb, uint32_t rc)
{
        reg[ra] = ~(reg[rb] & reg[rc]);
}

/* halt computation, op = 7 */
void halt(Mem_T memory)
{
        free_Mem(memory);
        exit(EXIT_SUCCESS);
}

/* map new segment, num_words = c, initialized to 0, 
* if not all 0's, and not currently mapped, put in seg and reg b
* op = 8 */
void activate(Mem_T memory, uint32_t reg[], uint32_t rb, uint32_t rc)
{
        /* rc is the number of words */
        /* rb gets updated to the new seg's ID */
        reg[rb] = Mem_map(memory, reg[rc]);
}

/* unmap seg/reg c, can now be used for something new, op = 9 */
void inactivate(Mem_T memory, uint32_t reg[], uint32_t rc) 
{
        Mem_unmap(memory, reg[rc]);
}

/* ouput: immediately print what is in register c, op = 10 */
void out(uint32_t reg[], uint32_t rc)
{
        /* print rc to IO (use write.h to do this) */
        output(reg[rc]);
}

/* reg c loaded with input, (value 0-255)
* if end of input, then reg c = full 32 bit word, every bit is 1 
* op 11 */
void input(uint32_t reg[], uint32_t rc)
{
        /* returns int from 0 - 255 unless it's at end of input
        * in which case it returns -1 */
        int32_t input = 0;
        input = get_input();

        /* check if we're at end of input */
        uint32_t all = 0;
        all = ~all;

        /* if we are at end of input, set reg c to all 1's */
        if(input == ENDFILE) {
                reg[rc] = all;
        }
        else {
                reg[rc] = input;
        }
}

/* seg/reg b duplicated, replaces seg 0, program pointer set to 
* seg 0, reg c, op 12 */
void loadp(Mem_T memory, uint32_t reg[], uint32_t rb)
{
        Mem_copy(memory, reg[rb]);
}

/* load value to reg c, op = 13 */
void load_val(uint32_t reg[], uint32_t rc, uint32_t value)
{
        reg[rc] = value;
}
