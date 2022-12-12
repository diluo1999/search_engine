/**
 * word.c - CS50 'word' module
 * 
 * A 'word' module is a common module between 'indexer' and 'querier' that implements "normalizeWord".
 * 
 * Di Luo
 * CS 50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mem.h"
#include "word.h"

/**************** function ****************/

/**************** normalizeWord() ****************/
/* See word.h for details */
char *normalizeWord(const char* word) {
    if (word != NULL) {
        char *normalizedWord = mem_malloc_assert(strlen(word)+1, "malloc() for normalizeWord failed");
        for (int i=0; i<(strlen(word)+1); i++) {
            normalizedWord[i] = tolower(word[i]);
        }
        return normalizedWord;
    } else {
        return NULL;
    }
}