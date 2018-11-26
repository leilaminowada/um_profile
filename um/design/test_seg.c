/* 
*
* tests for segmented memory implementation
* compiles against memory_seg.h (which includes segments.h)
* to test all of the functions for our memory
* HW6 COMP40
* 
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
//#include "memory_seg.h"

/* the struct is actually included in the memory_seg.h
* interface and would compile without this declaration
* when they link properly */
struct memory
{
	Seq_T ids; /* seq of 32 bit ids */
	Seq_T segments; /* seq of seq of 32 bit words */
};
typedef struct memory *Memory_T;

/* functions from memory_seg.h that we are testing */
extern void map_seg(int num_words, Memory_T memory);

extern void unmap_seg(Memory_T mem, uint32_t segid);

extern uint32_t get_value(Memory_T mem, uint32_t segid, uint32_t offset);

extern void store_value(Memory_T mem, uint32_t value, uint32_t  segid, 
					uint32_t offset);

extern Memory_T initialize_memory();

extern void free_memory(Memory_T);

/* test functions in main */
void test_initialize();
void test_map();
void test_unmap();
void test_get();
void test_store();
void test_free();

int main() 
{
	test_initialize();
	test_map();
	test_unmap();
	test_get();
	test_store();
	test_free();

	return 0;
}

/* should print all 0's for seg 0 initialized */
void test_initialize()
{
	Memory_T memory = initialize_memory();
	Seq_T first = Seq_remlo(memory->segments);
	int length = Seq_length(first);
	for(int i = 0; i<length; i++) {
		fprintf(stderr, "%u", (uint32_t)(uintptr_t)Seq_remlo(first));
	}
}

void test_store_and_get()
{
	Memory_T memory = initialize_memory();
	store_value(memory, 5, 0, 0);
	uint32_t val = get_value(memory, 0, 0);
	if(val == 5)
	{
		fprintf(stderr, "TRUE\n");
	}
	else
	{
		fprintf(stderr, "FALSE\n");
	}
}

void test_map()
{
	Memory_T memory = initialize_memory();
	map_seg(5, memory);
	for(int i = 0; i < 5; i++) {
		uint32_t val = (uint32_t)(uintptr_t)Seq_remlo
						(Seq_get(memory->segments, 1));
		if(val != 0) {
			fprintf(stderr, "FALSE\n");
		}
		else {
			fprintf(stderr, "TRUE\n");
		}
	}
}

void test_unmap()
{
	Memory_T memory = initialize_memory();
	map_seg(5, memory);
	unmap_seg(memory, 1);
	int length = Seq_length(memory->segments);
	if(length != 1) {
		fprintf(stderr, "FALSE\n");
	}
	else {
		fprintf(stderr, "TRUE\n");
	}
}

void test_free()
{
	Memory_T memory = initialize_memory();
	free_memory(memory);
	assert(memory == NULL);
}











