/* 
*   segments.h
*	implements a single segment
*	SECRET: how a segment is created and how to access/edit parts of it
*	hw 6 comp40
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>

#ifndef SEGMENTS_H
#define SEGMENTS_H

/* creates sequence of words */
Seq_T create_segment(int num_words);

/* frees sequence memory */
void delete_segment(Seq_T seg);

/* get word at certain id */
uint32_t get_word(Seq_T seg, uint32_t id);

/* inserts word into spot */
void insert_word(Seq_T seg, uint32_t id, uint32_t value);

/* removes word from sequence, DOES NOT change size of sequence */
//dont't think we need this?
// void remove_word(Seq_T, uint32_t id);

#endif