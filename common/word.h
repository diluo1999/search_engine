/* 
 * word.h - header file for CS50 'word' module
 * 
 * A 'word' module is a common module between 'indexer' and 'querier' that implements "normalizeWord"
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>

/**************** functions ****************/

/**************** normalizeWord() ****************/
/* Normalize a word
 * 
 * Caller provides:
 *      valid string for word to be normalized.
 * We return:
 *      a copy of normalized string, 
 *      NULL if the given word is NULL or error exists
 * Caller is responsible for:
 *      freeing the normalized string when finished.
 */
char *normalizeWord(const char* word);

#endif // __WORD_H