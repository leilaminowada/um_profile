/* 
* write.c
*
* SECRET: how to write instructions to output
*
* HW 6 COMP40
*
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

const unsigned BYTE_WIDTH = 8;
const int INVALID = 256;

/* reads in 8 byte unsigned char and returns the char as a uint32_t */
int32_t get_input()
{
        /* read in unsigned char from stdin and set to uint32_t */
        int32_t word;
        word = getc(stdin);

        /* check if end of input has been signaled
        * if it is, return -1; */
        if(word == EOF) {
                word = INVALID;
        }
        else {
                /* make sure char is within 0 - 255 */
                assert(word >= 0 && word <= 255);
        }
        return word;
}

/* takes in unsigned char represented as uint32_t 
* and outputs it as a char to stdout */
void output(uint32_t word)
{
        /* make sure char is within 0 - 255 */
        /* putchar char word to stdout */
        putc(word, stdout);
}
