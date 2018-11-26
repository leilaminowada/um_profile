/************************************************************************
*
*                             bitpack.c
*
*       Assignment: Comp40 Homework 6: Universal Machine
*       Date: October 20, 2018
*       
*       This file contains the implementation of the bitpack module.
*       The functions in this module operation on integers of sizet32
*       bits, both signed and unsigned. An attempt to pack a given
*       value in an insufficient number of bits to represent the value
*       will result Bitpack_Overflow to be raised. The given width and
*       LSBs of each function must be no larger than 32 when added
*       together. All calculations of powers of 2 are computed with
*       shifts, as the pow() function cannot compute large powers of 2.
*       1 is used as the base on all shifts to compute the powers of 2.
*
*       EDIT: this implementation of bitpack was originally used for
*       arith, but has been modified to work for bitpacking of 
*       instructions for the hw6 universal machine.
*
************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "assert.h"
#include "bitpack_um.h"
#include "except.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

const unsigned WORD_LENGTH = 32;
const unsigned BASE = 1;

/* 
* check to see whether given unsigned integer can be represented by
* width number of bits
*/
bool Bitpack_fitsu(uint32_t n, unsigned width)
{
        assert(width != 0);

        /* compute powers of 2 with shifts */
        uint32_t base = BASE;
        uint32_t exponent = width;
        uint32_t max_value = base << exponent;

        /* 
        * 2^32 can't be computed as signed integer; create
        * mask with allt32 bits being 1s by 'not'-ing a mask of 0s
        */
        if (width == WORD_LENGTH) {
                uint32_t mask = 0;
                max_value = ~mask;
        }

        if (n <= max_value) {
                return true;
        } else {
                return false;
        }
}

/* 
* check to see whether given signed integer can be represented by
* width number of bits
*/
// bool Bitpack_fitss(int32_t n, unsigned width)
// {
//         assert(width != 0);

//         /* compute powers of 2 with shifts */
//         int32_t base = BASE;
//         int32_t exponent = width - 1;
//         int32_t max_value = base << exponent;
//         int32_t min_value = max_value * -1;

         
//         * 231 can't be computed as signed integer; create
//         * mask with unsigned integer instead
        
//         if (width == WORD_LENGTH) {
//                 uint32_t mask = base << exponent;
//                 max_value = mask - 1;
//                 min_value = max_value * -1;
//         }

//         if (n < max_value && n >= min_value) {
//                 return true;
//         } else {
//                 return false;
//         }

// }

/* 
* get bits from the given word starting at bit lsb and ending at
* bit lsb + width - 1. interpret bits as unsigned.
*/
uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb)
{
        assert(width <= WORD_LENGTH);
        assert((width + lsb) <= WORD_LENGTH);

        /* compute powers of 2 with shifts */
        uint32_t base = BASE;
        uint32_t exponent_1 = width + lsb;
        uint32_t exponent_2 = lsb;
        uint32_t shift_1 = base << exponent_1;
        uint32_t shift_2 = base << exponent_2;

        uint32_t mask = shift_1 - shift_2;

        /* 
        * highest power of 2 computer can compute is 231. if 32 is
        * computed, the computer produces 1. therefore, if width is 32, 
        * mask should be all ones even though computer fails in the
        * computation above 
        */
        if (width == WORD_LENGTH)
        {
                mask = 0;
                mask = ~mask;
        }

        mask = mask & word;
        mask = mask >> lsb;
        return mask;
}

/* 
* get bits from the given word starting at bit lsb and ending at
* bit lsb + width - 1. interpret bits as signed. as set unused bits
* of returned int32_t to 0s or 1s according to the value of the most
* significant bit in the calculated bit field.
*/
// int32_t Bitpack_gets(uint32_t word, unsigned width, unsigned lsb)
// {
//         assert(width <= WORD_LENGTH);
//         assert((width + lsb) <= WORD_LENGTH);

//         /* compute powers of 2 with shifts */
//         uint32_t base = BASE;
//         uint32_t exponent_1 = width + lsb;
//         uint32_t exponent_2 = lsb;
//         uint32_t shift_1 = base << exponent_1;
//         uint32_t shift_2 = base << exponent_2;

//         uint32_t mask = shift_1 - shift_2;

//         /* 
//         * highest power of 2 computer can compute is 2^31. if 2^32 is
//         * computed, the computer produces 1. therefore, if width is 32, 
//         * mask should be all ones even though computer fails in the
//         * computation above 
//         */
//         if (width == WORD_LENGTH)
//         {
//                 mask = 0;
//                 mask = ~mask;
//         }

//         mask = mask & word;     
//         mask = mask >> lsb;

//         /* 
//         * determine whether the most significant bit in the bit field is
//         * a 0 or 1. Interpret as negative number if 1, and positive number
//         * otherwise.
//         */
//         uint32_t exponent_3 = width + lsb - 1;
//         uint32_t last_bit = base << exponent_3;
//         if ((word & last_bit) == last_bit) {
//                 if (width == WORD_LENGTH)
//                 {
//                         return mask;
//                 }
//                 int32_t exponent_4 = width;
//                 int32_t mask_2 = base << exponent_4;
//                 mask_2 = mask_2 * -1;
//                 mask_2 = mask_2 | mask;   
//                 return mask_2; 
//         } else {
//                 return mask;
//         }
// }

/* 
* returns a uint32_t with the bits starting at bit lsb and ending at bit
* lsb + width - 1 as the given unsigned value. Does not modify other bits
* in the given word.
*/
uint32_t Bitpack_newu(uint32_t word, unsigned width, unsigned lsb, uint32_t
                      value)
{
        /* ensure given value fits in given width */
        assert(width <= WORD_LENGTH);
        assert((width + lsb) <= WORD_LENGTH);
        bool fits = Bitpack_fitsu(value, width);
        if (fits == false) {
                RAISE(Bitpack_Overflow);
        }

        /* compute powers of 2 with shifts */
        uint32_t base = BASE;
        uint32_t exponent_1 = width + lsb;
        uint32_t exponent_2 = lsb;
        uint32_t shift_1 = base << exponent_1;
        uint32_t shift_2 = base << exponent_2;

        uint32_t mask_orig = shift_1 - shift_2;

        /* 
        * highest power of 2 computer can compute is 231. if 2t32 is
        * computed, the computer produces 1. therefore, if width ist32, 
        * mask should be all ones even though computer fails in the
        * computation above 
        */
        if (width == WORD_LENGTH) {
                mask_orig = 0;
                mask_orig = ~mask_orig;
        }

        uint32_t mask_opp = ~mask_orig;
        
        /* shifts bits of value to correct lsb position in new uint32_t */
        value = value << lsb;
        /* put zeros in bits being updated, and 'or' new ones in */
        mask_opp = mask_opp & word;
        mask_opp = mask_opp | value;

        return mask_opp;



}

/* 
* returns a uint32_t with the bits starting at bit lsb and ending at bit
* lsb + width - 1 as the given unsigned value. Does not modify other bits
* in the given word.
*/
// uint32_t Bitpack_news(uint32_t word, unsigned width, unsigned lsb, int32_t
//                       value)
// {
//         assert(width <= WORD_LENGTH);
//         assert((width + lsb) <= WORD_LENGTH);
//         bool fits = Bitpack_fitss(value, width);
//         if (fits == false) {
//                 RAISE(Bitpack_Overflow);
//         }

//         /* compute powers of 2 with shifts */
//         uint32_t base = BASE;
//         uint32_t exponent_1 = width + lsb;
//         uint32_t exponent_2 = lsb;
//         uint32_t shift_1 = base << exponent_1;
//         uint32_t shift_2 = base << exponent_2;

//         uint32_t mask_orig = shift_1 - shift_2;

//         /* 
//         * highest power of 2 computer can compute is 231. if 2t32 is
//         * computed, the computer produces 1. therefore, if width ist32, 
//         * mask should be all ones even though computer fails in the
//         * computation above 
//         */
//         if (width == WORD_LENGTH)
//         {
//                 mask_orig = 0;
//                 mask_orig = ~mask_orig;
//         }

//         uint32_t mask_opp = ~mask_orig; 

//         if (value < 0) {
//                 /* shifts value's bits to lsb position in new uint32_t */
//                 value = value << lsb;
//                 /* if value is negative, set unused bits in int32_t to 1s */
//                 value = value & mask_orig;
//                 /* put zeros in bits being updated, and 'or' new ones in */
//                 mask_opp = mask_opp & word; 
//                 mask_opp = mask_opp | value;

//         } else {
//                 /* shifts value's bits to lsb position in new uint32_t */
//                 value = value << lsb;
//                 /* put zeros in bits being updated, and 'or' new ones in */
//                 mask_opp = mask_opp & word;
//                 mask_opp = mask_opp | value;
//         }

//         return mask_opp;
// }





