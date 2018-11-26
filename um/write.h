/* 
* write.h
*
* SECRET: how to write instructions to output
*
* HW 6 COMP40
*
*/


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include "bitpack_um.h"

#ifndef WRITE_H
#define WRITE_H

/* reads in 8 byte unsigned char and returns
* the char as a uint32_t */
int32_t get_input();

/* takes in unsigned char represented as uint32_t 
* and outputs it as a char to stdout */
void output(uint32_t word);

#endif