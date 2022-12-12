/**
 * indexer.c - CS50 'indexer' module
 * 
 * A 'indexer' module reads the document files produced by the TSE crawler, 
 * builds an index, and writes that index to a file.
 * 
 * usage: ./indexer pageDirectory indexFilename
 * 
 * Assumption:
 * 1. pageDirectory has files named 1, 2, 3, …, without gaps
 * 2. The content of files in pageDirectory follow the format as defined in the specs; 
 * thus your code (to read the files) need not have extensive error checking.
 * 
 * Di Luo
 * CS 50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagedir.h"
#include "index.h"
#include "word.h"

#include "mem.h"
#include "webpage.h"
#include "counters.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
/* declarations */
// static void logr(const char *word, const int depth, const char *url);
static index_t *indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);

// /**************** logr ****************/
// // log one word (1-9 chars) about a given url                                   
// static void logr(const char *word, const int depth, const char *url)
// {
// #ifdef APPTEST
//   printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
// #else
//   ;
// #endif
// }

/**************** indexBuild() ****************/
/**
 * create index with webpages stored in pageDirectory
 * 
 * Caller privides:
 *      valid string for pageDirectory storing webpages.
 * We do:
 *      loop over all webpages stored by crawler in pageDirectory, 
 *      index those pages by looking for each word on each page, 
 *      make entry in index (hashtable) for each word.
 * We return:
 *      pointer to index that stores words in the webpages with respective counters,
 *      NULL if invalid pageDirectory or memory error. 
*/
static index_t *indexBuild(const char* pageDirectory) {
    index_t* index = mem_assert(index_new(500), "out of memory");
    int docID = 1;
    webpage_t* page = mem_assert(pagedir_load(pageDirectory, docID), "failed to load pageDirectory/1");
    while (page != NULL) {
        indexPage(index, page, docID);
        webpage_delete(page);
        docID++;
        page = pagedir_load(pageDirectory, docID);
    }
    return index;
}

/**************** indexPage() ****************/
/**
 * Find words on a single page, create a counter for each word, make entry into index. 
 * 
 * Caller privides:
 *      valid pointer to index,
 *      valid pointer to webpage,
 *      valid int for document ID.
*/
static void indexPage(index_t* index, webpage_t* page, const int docID) {
    char* word;
    char* normWord;
    int pos = 0;
    counters_t* counters;

    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word)>2) {
            normWord = normalizeWord(word);
            if ((counters = index_find(index, normWord)) == NULL) {
                counters = counters_new();
                index_insert(index, normWord, counters);
            }
            counters_add(counters, docID);
            mem_free(normWord);
        }
        mem_free(word);
    }
}

/**************** global functions ****************/
/* visible outside this file */

/**************** main() ****************/
/**
 * Parse the command line and validate parameters, 
 * build the index in memory by processing each webpage in pageDirectory, 
 * save index to file with indexFilename, 
 * and clean up data structures.
*/
int main(const int argc, char* argv[]) {
    // parse the command line and validate parameters
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", argv[0]);
        return 1;
    }

    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "invalid pageDirectory: %s\n", pageDirectory);
        return 2;
    }

    FILE* fp1 = fopen(indexFilename, "r");
    if (fp1 == NULL) {
        fprintf(stderr, "Cannot open file %s in reading mode\n", indexFilename);
        return 3;
    }
    fclose(fp1);

    FILE* fp2 = fopen(indexFilename, "w");
    if (fp2 == NULL) {
        fprintf(stderr, "Cannot open file %s in writing mode\n", indexFilename);
        return 4;
    }
    fclose(fp2);

    // build the index in memory by processing each webpage in pageDirectory
    index_t *index = indexBuild(pageDirectory);

    // save index to file with indexFilename
    index_save(index, indexFilename);

    // clean up data structures
    index_delete(index);

    return 0;
}