/*
*	get_instructions.h
*	does: unbitpacks instrutions that were passed
* 	to the driver and calls the correct execution
*
* 	secret: unbitpacking
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "instructions.h"
#include "bitpack_um.h"

#ifndef GINST_H
#define GINST_H

/* takes instruction, unbitpacks correct things */
uint32_t unpack_inst(Mem_T memory, uint32_t reg[], uint32_t instruction);

#endif



