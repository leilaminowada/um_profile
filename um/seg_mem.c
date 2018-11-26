#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>    
#include <uarray.h>
#include <assert.h>
#include "seq.h"
#include "seg_mem.h"

const int HINT = 1;
const uint32_t ZERO = 0;

/*
 *  * segmented memory is represented with two sequences, one which
 *  * contains pointers to individual memory segments, and another which
 *  * contains the set of unmapped indicies. the pointer at a given index
 *  * of the mapped segment sequence is null if the segment is unmapped. 
 *  * the unmapped segment id sequence is used as an unboxed adt where ids
 *  * are cast onto void pointers.
 *  */
struct Mem_T {
	Seq_T mapped_segments;
	Seq_T unmapped_segment_ids;	
};

/* struct for one segment in memory */
typedef struct Segment {
	uint32_t *words;
	uint32_t length;
} *Segment;

/* initialize a segment */
Segment segment_init(uint32_t length)
{
	/* allocate new segment */
	Segment seg = malloc(sizeof(struct Segment));

	/* initialize length */
	seg->length = length;

	/* initialize c-array of words to 0 */
	seg->words = calloc(length, sizeof(uint32_t));

	return seg;
}

/*
 *  * initializes memory for the sequence of segments, the sequence of 
 *  * unmapped segment ids, and the Mem_T struct
 *  */
Mem_T new_Mem()
{
	/* malloc for memory struct */
	Mem_T memory = malloc(sizeof(struct Mem_T));

	/* initialize both sequences */
	Seq_T mapped = Seq_new(HINT);
	Seq_T unmapped = Seq_new(HINT);
	memory->mapped_segments = mapped;
	memory->unmapped_segment_ids = unmapped;

	return memory;
}

/*
 * * creates a new memory segment and adds it to the sequence
 * * of memory segments. the segment is either appended to the 
 * * end of the sequence, if the sequence has no free indices, and
 * * given an id equal to the length of the sequence or is added 
 * * at the least recently unmapped index and given the corresponding id.
 * */
segment_Id Mem_map(Mem_T mem, uint32_t length)
{
	/* seg id that we will return */
	segment_Id id = 0;

	/* initialize new segment */
	Segment seg = segment_init(length);

	/* check if there are unmapped ids */
	int id_len = Seq_length(mem->unmapped_segment_ids);
	if(id_len != 0) {
		id = (uint32_t)(uintptr_t)Seq_remhi(mem->unmapped_segment_ids);
		Seq_put(mem->mapped_segments, id, seg);
	}
	else {
		id = Seq_length(mem->mapped_segments);
		Seq_addhi(mem->mapped_segments, seg);
	}
	return id;
}

/*
 *  * unmaps the memory segment from the sequence, leaving the id of the 
 *  * corresponding index free. the free id is then added to the
 *  * queue of unmapped segment ids. zero can never be added to the 
 *  * sequence of unmapped ids.
 *  */
void Mem_unmap(Mem_T mem, segment_Id id)
{
	/* get segment at id from mapped_segments */
	Segment unmap = Seq_get(mem->mapped_segments, id);

	/* free memory segment and its words */
	free(unmap->words); //am i freeing this correctly?
	free(unmap);
	
	/* put NULL at the id index in the mapped segment seq */
	Seq_put(mem->mapped_segments, id, NULL);

	/* add id to unmapped segments sequence (needs void * cast) */
	Seq_addhi(mem->unmapped_segment_ids, (void *)(uintptr_t)id);
}

/* load a value from memory */
uint32_t Mem_load(Mem_T mem, segment_Id id, uint32_t offset)
{	
	/* get segment at id from mapped_segments */
	Segment seg = Seq_get(mem->mapped_segments, id);

	//fprintf(stderr, "WORD LOADED: %u\n", seg->words[offset]);
	/* return word at offset in segment words array*/
	return seg->words[offset];
}

/* store a value at a spot in memory */
void Mem_store(Mem_T mem, segment_Id id, uint32_t offset, uint32_t value)
{
	/* get segment at id from mapped_segments */
	Segment seg = Seq_get(mem->mapped_segments, id);

	/* segment words at offest gets value */
	seg->words[offset] = value;
}

/* * makes a deep copy of the segment at the index from and stores
 * * the segment at the index specified by into. if there is a segment
 * * stored at the into index it is removed from the sequence and freed. */
void Mem_copy(Mem_T mem, segment_Id copy_id)
{ 
	/* if trying to load copy_id = 0, just return */
	if(copy_id == ZERO) {
		//fprintf(stderr, "ITS ZERO\n");
		return;
	}

	/* get segment at copy id from mapped segments */
	Segment seg = Seq_get(mem->mapped_segments, copy_id);

	/* initialize copy of the segment with segment_init */
	Segment copy = segment_init(seg->length);

	/* copy segment words into copy words using for loop */
	int seglen = seg->length;
	for(int i = 0; i < seglen; i++) {
		copy->words[i] = seg->words[i];
	}

	/* get the zero segment from mapped segments */
	Segment zero = Seq_get(mem->mapped_segments, ZERO);
	
	/* free the zero segment and its words */
	free(zero->words);
	free(zero);
	
	/* put copied segment into id 0 in mapped segments */
	Seq_put(mem->mapped_segments, ZERO, copy);
}

/*
 *  * Frees all allocated segements, the sequence of segments, the 
 *  * sequence of 
 *  * unmapped segment ids, and the Mem_T struct
 *  */
void free_Mem(Mem_T mem)
{
	/* get length of mapped segments */
	int maplen = Seq_length(mem->mapped_segments);
	
	/* free the mapped segments */
	for(int i = 0; i < maplen; i++) {
		Segment seg = Seq_get(mem->mapped_segments, i);
		if(seg != NULL) {
			/* free memory segment and its words */
			free(seg->words); //am i freeing this correctly?
			free(seg);
		}
	}

	/* free the both sequences */
	Seq_free(&(mem->mapped_segments));
	Seq_free(&(mem->unmapped_segment_ids));
	free(mem);
}












