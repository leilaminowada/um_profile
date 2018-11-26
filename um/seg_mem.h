#ifndef SEGMENTED_MEMORY_INCLUDED
#define SEGMENTED_MEMORY_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "seq.h"        
   
typedef struct Mem_T *Mem_T;
typedef uint32_t segment_Id; 

/*
 *  * Returns the pointer to new segemented memory
 *   */
Mem_T new_Mem();

/*
 *  * Creates a new memory segment with the designated length and
 *   * returns the id of the new memory segment
 *    */
segment_Id Mem_map(Mem_T mem, uint32_t length);

/*
 *  * Unmaps the memory segment at the specified id
 *   */
void Mem_unmap(Mem_T mem, segment_Id id);

/*
 *  * Load the value at at the specified id and offset
 *   */
uint32_t Mem_load(Mem_T mem, segment_Id id, uint32_t offset);

/*
 *  * Stores value at the specified id and offset
 *   */
void Mem_store(Mem_T mem, segment_Id id, uint32_t offset, 
					    uint32_t value);

/*
 *  * Copies segment at the from id and replaces the segment at the 
 *    * into id with the copied segment
 *     */
void Mem_copy(Mem_T mem, segment_Id copy_id);

/*
 *  * Frees the entire segmented memory that was created by
 *   * an instance of new_mem()
 *    */
void free_Mem(Mem_T mem);

#endif

