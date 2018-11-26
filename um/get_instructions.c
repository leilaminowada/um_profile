/*
*	get_instructions.h
*	does: unbitpacks instrutions that were passed
* 	to the driver and calls the correct execution
*
* 	secret: unbitpacking
*
*	hw 6 comp40
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "instructions.h"
#include "bitpack_um.h"
#include "seg_mem.h"

uint32_t const OP_SIZE = 4;
uint32_t const OP_LSB = 28;
uint32_t const REG_SIZE = 3;
uint32_t const VAL_SIZE = 25;

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* takes instruction, unbitpacks correct things */
int32_t unpack_inst(Mem_T memory, uint32_t reg[], uint32_t instruction)
{
	/* unbitpack the opcode */
	uint32_t opcode = Bitpack_getu(instruction, OP_SIZE, OP_LSB);

	if(opcode == CMOV) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			cmov(reg, ra, rb, rc);
	}
	else if (opcode == SLOAD) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			sload(memory, reg, ra, rb, rc);
	}
		else if (opcode == SSTORE) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			sstore(memory, reg, ra, rb, rc);
	}
		else if (opcode == ADD) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			add(reg, ra, rb, rc);
	}
		else if (opcode == MUL) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			mul(reg, ra, rb, rc);
	}
		else if (opcode == DIV) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			divide(reg, ra, rb, rc);
	}
		else if (opcode == NAND) {
			uint32_t ra = Bitpack_getu(instruction, REG_SIZE, 6);
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			nand(reg, ra, rb, rc);
	}
		else if (opcode == HALT) {
			halt(memory);
			return HALT;
	}
		else if (opcode == ACTIVATE) {
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			activate(memory, reg, rb, rc);
	}
		else if (opcode == INACTIVATE) {
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			inactivate(memory, reg, rc);
	}
		else if (opcode == OUT) {
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			out(reg, rc);
	}
		else if (opcode == IN) {
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			input(reg, rc);
	}
		else if (opcode == LOADP) {
			uint32_t rb = Bitpack_getu(instruction, REG_SIZE, 3);
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 0);
			loadp(memory, reg, rb);
			return reg[rc];
	}
		else if (opcode == LV) {
			uint32_t rc = Bitpack_getu(instruction, REG_SIZE, 25);
			uint32_t value = Bitpack_getu(instruction, VAL_SIZE, 0);
			load_val(reg, rc, value);
	}
	return -1;

}	
















