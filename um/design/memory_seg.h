/* 
*   memory_seg.h
*
*	uses the segments.h interface to create and use
*	segmented memory as well as keep track of segments
*	that have been unmapped
*
* 	point: don't show the user that they are interfacing with 
* 	a sequence --> just show the struct memory, so it looks
* 	to them as though they are interfacign with memory
*
*	SECRET: how memory has been segmented and stored
*
*	comp40 hw6
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include "segments.h"

#ifndef MEMORY_SEG
#define MEMORY_SEG

/* ask about hiding this typdeffed */
struct memory
{
	Seq_T ids; /* seq of 32 bit ids */
	Seq_T segments; /* seq of seq of 32 bit words */
};
typedef struct memory *Memory_T;

/* maps a segment */
void map_seg(int num_words, Memory_T memory);

/* unmaps a segment */
void unmap_seg(Memory_T mem, uint32_t segid);

/* returns to user value at specific segment/word */
uint32_t get_value(Memory_T mem, uint32_t segid, uint32_t offset);

/* updates value at specified segment/word */
void store_value(Memory_T mem, uint32_t value, uint32_t  segid, 
					uint32_t offset);

/* initializes id sequence and segment sequence */
/* returns struct with seq of ids (0) and seq of segments 
*  (seg 0 is initialized) */
Memory_T initialize_memory();

/* frees sequences in struct */
void free_memory(Memory_T);


#endif

