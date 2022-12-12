/* 
 * index.h - header file for CS50 'index' module
 * 
 * A 'index' module is a wrapper around hashtable that keyed by word and 
 * storing counters as items in hashtable's set. The module has wrapper 
 * functions that call their corresponding funcitons in 'hashtable' module. 
 * In addition, the module provides functions to save an index to disk 
 * and to load it into memory from disk.
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include "hashtable.h"
#include "counters.h"

/**************** global types ****************/
/* create index_t from hashtable_t (index_t is essentially a wrapper over hashtable_t) */
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 * 
 * Caller provides:
 *      number of slots to be used for the index (must be > 0).
 * We return:
 *      pointer to a new index, or NULL if error.
 * We guarantee:
 *      index is initialized empty.
 * Caller is responsible for:
 *      later calling index_delete.
 */
index_t *index_new(int number_slots);

/**************** index_insert ****************/
/* Insert pointer to counters, identified by key (word), into the given index
 * 
 * Caller provides:
 *      valid pointer to index, valid string for key (word), valid pointer for counters.
 * We return:
 *      false if word exists in index, any parameter is NULL, or error;
 *      true iff new counters was inserted.
 * Notes:
 *      The key string is copied for use by the index; that is, the module
 *      is responsible for allocating memory for a copy of the key string, and
 *      later deallocating that memory; thus, the caller is free to re-use or 
 *      deallocate its key string after this call.  
 */
bool index_insert(index_t* index, const char* word, counters_t* counters);

/**************** index_find ****************/
/* Return the counters associated with the given word.
 *
 * Caller provides:
 *      valid pointer to index, valid string for word.
 * We return:
 *      pointer to the counters corresponding to the given word, if found;
 *      NULL if index is NULL, word is NULL, or word is not found.
 * Notes:
 *      the index is unchanged by this operation.
 */
counters_t* index_find(index_t* index, const char* word);

/**************** index_save ****************/
/* Save an index to disk
 * 
 * Caller provides:
 *      valid pointer to index, 
 *      valid string for the filename storing the index
 * We do:
 *      nothing, if index==NULL, indexFilename==NULL, or indexFilename is not a valid filename;
 *      otherwise, call hashtable_iterate() to save index in the file in the format:
 *      word docID count [docID count] ...
 */
void index_save(index_t *index, const char *indexFilename );

/**************** index_load ****************/
/* Load an index from disk
 *
 * Caller provides:
 *      valid string for the pathname of a file storing an index.
 * We return:
 *      NULL, if indexFilename is NULL, indexFilename is not a valid filename, or indexFilename is a empty file,
 *      otherwise, pointer to index built from the file.
 */
index_t *index_load(const char *indexFilename);

/**************** index_delete ****************/
/* Delete index, calling a delete function on each counters.
 * 
 * Caller provides:
 *      valid index pointer.
 * We do:
 *      if index==NULL, do nothing.
 *      otherwise, unless counters_delete==NULL, call counters_delete on each counters.
 *      free all the word strings and associated counters, and the index itself.
 * Notes:
 *      We free the strings that represent word for each item, because 
 *      this module allocated that memory in index_insert.
 */
void index_delete(index_t* index);

#endif // __INDEX_H