/* 
 * index.c - CS50 'index' module
 * 
 * See index.h for more information
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "index.h"
#include "mem.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** local functions ****************/
/* not visible outside this file */
/* declarations */
static void save_item(void *arg, const char *key, void *item);
static void save_counts(void *arg, const int key, const int count);

/**************** save_item() ****************/
/**
 * helper to print word and call counters_iterate()
 * 
 * Caller provides:
 *      valid pointer to file saving index,
 *      valid string for word (key)
 *      valid pointer to counters (item)
 * 
*/
static void save_item(void *arg, const char *key, void *items) {
    fprintf(arg, "%s ", key);
    counters_iterate(items, arg, save_counts);
    fprintf(arg, "\n");
}

/**************** save_counts() ****************/
/**
 * helper to print each (docID, count) pair in counters
*/
static void save_counts(void *arg, const int key, const int count) {
    fprintf(arg, "%d %d ", key, count);
}

/**************** ct_delete() ****************/
/**
 * helper to delete counters
*/
static void ct_delete(void *item)
{
	counters_t* ct = item;

	if (ct != NULL) {
		counters_delete(ct);
	}
}

/**************** global functions ****************/
/* visible outside this file */
/* see index.h for comments about exported functions */

/**************** index_new() ****************/
/* see index.h for description */
index_t *index_new(int number_slots) {
    return (index_t *) hashtable_new(number_slots);
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, const char* word, counters_t* counters) {
    return hashtable_insert((hashtable_t *)index, word, counters);
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t* index_find(index_t* index, const char* word) {
    return (counters_t *) hashtable_find((hashtable_t *)index, word);
}

/**************** index_save() ****************/
/* see index.h for description */
void index_save(index_t *index, const char *indexFilename ) {
    if (index != NULL) {
        if (indexFilename != NULL) {
            FILE* fp = fopen(indexFilename, "w");
            if (fp != NULL) {
                hashtable_iterate((hashtable_t *)index, fp, save_item);
            } else {
                fprintf(stderr, "Cannot open file: %s\n", indexFilename);
            }
            fclose(fp);
        } else {
            fprintf(stderr, "indexFilename is NULL\n");
        }
    } else {
        fprintf(stderr, "index is NULL\n");
    }
}

/**************** index_load() ****************/
/* see index.h for description */
index_t*
index_load(const char* indexFilename)
{
  if (indexFilename == NULL) {
    fprintf(stderr, "index: indexFilename NULL\n");
    return NULL;
  }
  FILE* fp = fopen(indexFilename, "r");
  if (fp == NULL) {
    fprintf(stderr, "index: cannot open index file '%s'\n", indexFilename);
    return NULL;
  }
  // how many words in the index?
  int wordcount = file_numLines(fp);
  // check if it's a empty file
  if (wordcount == 0) {
    fprintf(stderr, "index: cannot read lines from '%s'\n", indexFilename);
    return NULL;
  }
  // create an empty index with ~2words/slot; index_new never returns NULL
  index_t* index = index_new(wordcount/2+1);
  // read words from file
  char* word;
  while ( (word = file_readWord(fp)) != NULL ) {
    // found a word, now build a counters list for it
    counters_t* ctrs = mem_assert(counters_new(), "new counters");
    // now read (docId count) pairs into the counters list
    int docID, count;
    while (fscanf(fp, "%d %d ", &docID, &count) == 2) {
      if (!counters_set(ctrs, docID, count)) {
        fprintf(stderr, "index: unexpected, counters_set returned false\n");
      }
    }
    if (!hashtable_insert(index, word, ctrs)) {
      fprintf(stderr, "index: unexpected, hashtable_insert returned false\n");
    }
    free(word);
  }
  fclose(fp);
  return index;
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index) {
    hashtable_delete((hashtable_t *)index, ct_delete);
}