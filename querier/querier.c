/**
 * querier.c - CS50 'querier' module
 * 
 * A 'querier' module reads the index file produced by the TSE Indexer, 
 * and page files produced by the TSE Crawler, 
 * and answers search queries submitted via stdin.
 * 
 * usage: ./querier pageDirectory indexFilename
 * 
 * Assumption:
 * 1. pageDirectory has files named 1, 2, 3, …, without gaps
 * 2. The content of files in pageDirectory follow the format as defined in the specs; 
 * thus your code (to read the files) need not have extensive error checking.
 * 3. The content of the file named by indexFilename follows our index file format (as defined in Lab 5); thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
 * 4. The provided index file corresponds to the provided pageDirectory, that is, was built by indexer from the files in that directory.
 * 
 * Di Luo
 * CS 50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>  // add this to your list of includes

#include "pagedir.h"
#include "index.h"
#include "word.h"

#include "mem.h"
#include "file.h"
#include "counters.h"

/******** local data types *******/
struct twocounters {
    counters_t *orSequence;
    counters_t *otherSequence;
};

struct maxDoc {
    int *key;
    int *count;
};

/**************** functions ****************/
/* declarations */
/* The fileno() function is provided by stdio,
 * but for some reason not declared by stdio.h, so declare here.
 */
int fileno(FILE *stream);
static void prompt(void);
static void MERGE(counters_t** andSequence, counters_t** orSequence);
static void UNION(counters_t* orSequence, counters_t* otherSequence);
void unionFunction(void *arg, const int key, const int count);
static void INTERSECT(counters_t* orSequence, counters_t* otherSequence);
void intersectFunction(void *arg, const int key, const int count);
static char** parseQuery(char* rawQuery);
static counters_t* processQuery(char** parsedQuery, index_t* index);
void findmaxFunction(void *arg, const int key, const int count);
static void FINDMAX(counters_t* orSequence, int *key, int *count);
void counterFunction(void *arg, const int key, const int count);

static inline int min(const int a, const int b) {
  return (a < b ? a : b);
}

/**************** prompt() ****************/
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** unionFunction() ****************/
void unionFunction(void *arg, const int key, const int count) {
    // update arg counters 
    counters_t *ct = arg;
    int wordCount = counters_get(ct, key); // returns 0 if key not found
    counters_set(ct, key, count + wordCount);
}

/**************** UNION() ****************/
static void UNION(counters_t* orSequence, counters_t* otherSequence) {
    // union orSequence and otherSequence by adding counts for matching keys
    // store results in orSequence
    counters_iterate(otherSequence, orSequence, unionFunction);
}

/**************** intersectFunction() ****************/
void intersectFunction(void *arg, const int key, const int count) {
    struct twocounters *two = arg;
    counters_set(two->orSequence, key, min(count, counters_get(two->otherSequence, key)));
}

/**************** INTERSECT() ****************/
static void INTERSECT(counters_t* orSequence, counters_t* otherSequence) {
    // intersect orSequence and otherSequence by taking of counts for matching keys
    // store results in orSequence
    struct twocounters args = {orSequence, otherSequence};
    counters_iterate(orSequence, &args, intersectFunction);
}

/**************** MERGE() ****************/
static void MERGE(counters_t** andSequence, counters_t** orSequence) {
    // push temporary result andSequence to final result orSequence
    // also reset andSequence
    if (*andSequence != NULL) {
        if (*orSequence == NULL) {
            *orSequence = counters_new();
        }
        UNION(*orSequence, *andSequence); //union andSequence into orSequence
        counters_delete(*andSequence);
        *andSequence = NULL;
    }
}

/**************** parseQuery() ****************/
/* See specs for detail */
static char** parseQuery(char* rawQuery) {
    char* word = rawQuery;
    char* rest = "";
    char** parsedQuery = (char**)mem_calloc_assert(100, sizeof(char*), "failed to calloc when parsing query");
    int parsedQueryIndex = 0;

    for (int i=0; rawQuery[i]!='\0'; i++) {
        if (isspace(*word) == 0) { // word is char
            if ((isalpha(*rest) == 0)&&(isspace(*rest) == 0)) { // rest is \0
                rest = word;
                *rest = tolower(*rest);
                rest++;
                parsedQuery[parsedQueryIndex] = word;
                parsedQueryIndex++;
            }
            else if (isspace(*rest) != 0){ // rest is space
                *rest = '\0';
                word = rest;
                word++;
            }
            else { // rest is char
                *rest = tolower(*rest);
                rest++;
            }
        }
        else { // word is space
            word++;
        }
    }

    if (*rest == *rawQuery) { // the string is empty or only has space
        return NULL;
    }
   
    return parsedQuery;
}

/**************** processQuery() ****************/
/* See specs for detail.*/
static counters_t* processQuery(char** parsedQuery, index_t* index) {
    // Create two counters initially set to NULL:
    counters_t* andSequence = NULL; // holds result of current AND operations
    counters_t* orSequence = NULL;  // accumulates the final result combining OR sequences
    bool shortCircuit = false;    // stop early if AND sequence will fail

    // Loop over all words in query
    for (int i=0; parsedQuery[i]!=NULL; i++) {
        if (!strcmp(parsedQuery[i], "or")) {
            // orSequence holds final result, merge in andSequence seen so far
            // think of stepping back after previously stepping away to calculate AND
            MERGE(&andSequence, &orSequence); 
            shortCircuit = false;  //reset shortCircuit
            continue;
        }
        if (shortCircuit) {
            // at least one word returned no results, so AND sequence will fail
            // skip the words until see OR and shortCircuit is reset
            continue;
        }
        if (!strcmp(parsedQuery[i], "and")) {
            continue; // continue to next word, same as if AND wasn't there
        }
        else { // found non operator word 
            // think of stepping away to calculate AND sequence
            // get pages for this word with match = index_find
            counters_t* match = index_find(index, parsedQuery[i]);

            if (match == NULL) {  // no pages contain this query word
                shortCircuit = true; // AND will fail for next word, so skip it and others until see OR
                if (andSequence != NULL) {
                    // because this word returned no pages, drop everything AND'ed so far
                    counters_delete(andSequence);
                    andSequence = NULL;
                }
            }
            else { // found counters for this word in index
                if (andSequence == NULL) { // this is the first word of AND sequence
                    andSequence = counters_new();
                    UNION(andSequence, match);  //essentially copies match to andSequence
                }
                else { // not first word in AND sequence
                    INTERSECT(andSequence, match); //intersect this word's pages 
                }
            }
        }
    }
    
    MERGE(&andSequence, &orSequence);

    return orSequence;
}

/**************** findmaxFunction() ****************/
void findmaxFunction(void *arg, const int key, const int count) {
    struct maxDoc *max = arg;
    if (count > *(max->count)){
        *(max->count) = count;
        *(max->key) = key;
    }
}

/**************** FINDMAX() ****************/
static void FINDMAX(counters_t* orSequence, int *key, int *count) {
    *key = 1;
    *count = 0;
    struct maxDoc max = {key, count};
    counters_iterate(orSequence, &max, findmaxFunction);
}

/**************** counterFunciton() ****************/
void counterFunction(void *arg, const int key, const int count) {
    int *counter = arg;
    if (count > 0) {
        (*counter)++;
    }
}

/**************** main() ****************/
/* See specs for detail */
int main(const int argc, char* argv[]) {
    
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    // parse the command line and validate parameters
    // check if the number of parameter is correct
    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", argv[0]);
        return 1;
    }

    // check if pageDirectory parameter is valid
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "invalid pageDirectory: %s\n", pageDirectory);
        return 2;
    }

    // check if the file specified by indexFilename can be opened in reading mode
    FILE* fp = fopen(indexFilename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file %s in reading mode\n", indexFilename);
        return 3;
    }
    fclose(fp);

    index_t* index = index_load(indexFilename);

    // infinite loop that asks for queries until EOF or ctrl+D
    for (;;) {
        char* rawQuery;
        char** parsedQuery;
        int continueIndicator = 0;
        // char** 

        prompt();
        rawQuery = file_readLine(stdin);
        if (rawQuery == NULL) {
            printf("^D\n");
            break;
        }
        else {
            // check if all char in rawQuery are space or alphabet
            for (int i=0; rawQuery[i]!='\0'; i++) {
                if ((isalpha(rawQuery[i])==0)&&(isspace(rawQuery[i])==0)) {
                    fprintf(stderr, "Error: bad character '%c' in query.\n", rawQuery[i]);
                    continueIndicator = 1;
                    break;
                }
            }
            if (continueIndicator == 1) {
                mem_free(rawQuery);
                continue;
            }

            // tokenize the raw query
            parsedQuery = parseQuery(rawQuery);
            if (parsedQuery == NULL) {// the string is empty or only has space, ignore
                mem_free(rawQuery);
                continue;
            }

            // print clean query
            printf("Query: ");
            for (int i=0; parsedQuery[i] != NULL; i++) {
                printf("%s ", parsedQuery[i]);
            }
            printf("\n");

            // validate clean query
            // check if the first word is an operator
            if (!strcmp(parsedQuery[0],"and")||!strcmp(parsedQuery[0],"or")) {
                fprintf(stderr, "Error: '%s' cannot be first\n", parsedQuery[0]);
                mem_free(rawQuery);
                mem_free(parsedQuery);
                continue;
            }
            // loop through clean query
            for (int i=0; parsedQuery[i] != NULL; i++) {
                if (parsedQuery[i+1] == NULL) { // last word
                    // check if the last word is an operator
                    if (!strcmp(parsedQuery[i],"and")||!strcmp(parsedQuery[i],"or")) {
                        fprintf(stderr, "Error: '%s' cannot be last\n", parsedQuery[i]);
                        continueIndicator = 1;
                        break;
                    }
                } else { // check if there are two consecutive operators
                    if (!strcmp(parsedQuery[i],"and")||!strcmp(parsedQuery[i],"or")) {
                        if (!strcmp(parsedQuery[i+1],"and")||!strcmp(parsedQuery[i+1],"or")) {
                            fprintf(stderr, "Error: two operators '%s' and '%s' cannot be adjacent\n", parsedQuery[i], parsedQuery[i+1]);
                            continueIndicator = 1;
                            break;
                        }
                    }
                }
            }
            if (continueIndicator == 1) {
                mem_free(rawQuery);
                mem_free(parsedQuery);
                continue;
            }

            // process clean and validated query
            counters_t* result = processQuery(parsedQuery, index);

            // counters_print(result, stdout);
            int key;
            int count;
            int counter = 0;
            webpage_t* web;

            // computer the number of matched documents
            counters_iterate(result, &counter, counterFunction);
            printf("Matches %d documents (ranked):\n", counter);

            // use search sort to find the doc with the largest count
            // in every iteration, print the doc, and set its count to zero.
            
            for (int i=0; i<counter; i++) {
                FINDMAX(result, &key, &count);
                web = pagedir_load(pageDirectory, key);
                printf("score%4d doc%4d: %s\n", count, key, webpage_getURL(web));
                counters_set(result, key, 0);
                webpage_delete(web);
                web = NULL;
            }
            
            counters_delete(result);
            printf("-----------------------------------------------\n");
        }

        mem_free(rawQuery);
        mem_free(parsedQuery);
    }
    
    index_delete(index);

    return 0;
}