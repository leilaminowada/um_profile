/*
*	bitpack_um.h
*
*	interface for bitpacking of instructions for UM
*	and we only use unsigned packing/unpacking because
*	all of our words are stored as unsigned
*
*	SECRET: how to bitpack information into instruction
*
* 	HW 6, COMP40
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "assert.h"
#include "except.h"

#ifndef BITPACK_H
#define BITPACK_H

/* checks if num will fit in unsigned 32 bit int */
bool Bitpack_fitsu(uint32_t n, unsigned width);

/* checks if num will fit in signed 32 bit */
//bool Bitpack_fitss(int32_t n, unsigned width);

/* gets specific bits out of unsigned word */
uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb);

/* get specific bits out of signed word */
//int32_t Bitpack_gets(uint32_t word, unsigned width, unsigned lsb);

/* get new unsigned int */
uint32_t Bitpack_newu(uint32_t word, unsigned width, unsigned lsb, uint32_t
                      value);

/* get new signed int */
//uint32_t Bitpack_news(uint32_t word, unsigned width, unsigned lsb, int32_t
//                      value);

#endif