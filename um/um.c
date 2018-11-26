/*
*	um.c
*	basically driver for um
*	comp40 hw 6
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "seq.h"
#include "seg_mem.h"
#include "write.h"
#include "get_instructions.h"
#include "bitpack_um.h"

const int INST_SIZE = 4;
const int MAIN = 0;
const uint32_t CHAR_SIZE = 8;

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* --------main function to run the UNIVERAL MACHINE -------*/
int main(int argc, char *argv[])
{
	/* function variables */
	(void) argc;
	long int bsize = 0, file_size = 0, pcounter = 0;;
	uint32_t value = 0;

	/* memory variables */
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL) {
		fprintf(stderr, "Incorrect input format.\n");
		exit(EXIT_FAILURE);
	}
	uint32_t registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	/* get the length of the program */
	fseek(fp, 0L, SEEK_END);
	bsize = ftell(fp); 	    /* this is file size in bytes */
	file_size = bsize/INST_SIZE;  /* num of 32 bit ints in file*/
	fseek(fp, 0L, SEEK_SET);    /* reset the pointer */

	Mem_T memory = new_Mem();
	Mem_map(memory, file_size);

	/* put all the instructions into segment 0 */
	for(long int i = 0; i < file_size; i++) {
		value = Bitpack_newu(value, CHAR_SIZE, 24, fgetc(fp));
		value = Bitpack_newu(value, CHAR_SIZE, 16, fgetc(fp));
		value = Bitpack_newu(value, CHAR_SIZE, 8, fgetc(fp));
		value = Bitpack_newu(value, CHAR_SIZE, 0, fgetc(fp));

		Mem_store(memory, MAIN, i, value);
	}
	fclose(fp);

	/* keep running infinite loop */
	while(1) {
		
		/* retreive instruction from word we're at according
		*  to counter */
		uint32_t inst = Mem_load(memory, MAIN, (uint32_t)pcounter);
		pcounter++;
		int32_t num = unpack_inst(memory, registers, inst);

		/* check if we need to do anything according to
		*  instruction */
		if(num != -1) {
			pcounter = num; //rc
		}
	}
	return 0;
}



















