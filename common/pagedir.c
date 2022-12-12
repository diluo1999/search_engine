/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * A 'pagedir' module 
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "pagedir.h"
#include "file.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
/* declarations */
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t *pagedir_load(const char* pageDirectory, const int docID);

/**************** pagedir_init ****************/
/* See pagedir.h for details */
bool pagedir_init(const char* pageDirectory) 
{
    // construct the pathname for the .crawler file in that directory
    char pathname[100];
    snprintf(pathname, 100, "%s%s", pageDirectory, "/.crawler");
    
    // open the file for writing; on error, return false.
    FILE* fp = fopen(pathname, "w");
    if (fp == NULL) {
        return false;
    }

    // close the file and return true.
    fclose(fp);
    return true;
}

/**************** pagedir_save ****************/
/* See pagedir.h for details */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) 
{
    // construct the pathname for the page file in pageDirectory
    char pathname[100];
    snprintf(pathname, 100, "%s/%d", pageDirectory, docID);

    // open that file for writing
    FILE* fp = fopen(pathname, "w");

    // print the URL, depth, and contents of the webpage
    fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    // close the file
    fclose(fp);
}

/**************** pagedir_validate ****************/
/* See pagedir.h for details */
bool pagedir_validate(const char* pageDirectory) {
    if (pageDirectory != NULL) {
         // construct the pathname for the .crawler file in that directory
        // char* pathname = mem_malloc_assert(sizeof(pageDirectory)+sizeof("/.crawler")+sizeof(char), "malloc for pathname failed");
        // snprintf(pathname, sizeof(pageDirectory)+sizeof("/.crawler")+2*sizeof(char), "%s%s", pageDirectory, "/.crawler");
        char pathname[100];
        snprintf(pathname, 100, "%s%s", pageDirectory, "/.crawler");

        // open the file for reading; on error, return false.
        FILE* fp = fopen(pathname, "r");
        if (fp == NULL) {
            return false;
        } else {
            fclose(fp);
            return true;
        }
    } else {
        return false;
    }
}

/**************** pagedir_load ****************/
/* See pagedir.h for details */
webpage_t *pagedir_load(const char* pageDirectory, const int docID) {
    if (pageDirectory != NULL && docID > 0) {
        // construct the pathname for the page file in that directory
        // char* pathname = mem_malloc_assert(sizeof(pageDirectory)+3*sizeof(char), "malloc for pathname failed");
        // snprintf(pathname, sizeof(pageDirectory)+4*sizeof(char), "%s/%d", pageDirectory, docID);
        char pathname[100];
        snprintf(pathname, 100, "%s/%d", pageDirectory, docID);

        // open that file for writing
        FILE* fp = fopen(pathname, "r");
        if (fp == NULL) {
            return NULL;
        }

        char* url = file_readLine(fp);
        char* depthBuffer = file_readLine(fp);
        int depth = atoi(depthBuffer);
        mem_free(depthBuffer);
        char* html = file_readFile(fp);

        webpage_t* webpage = webpage_new(url, depth, html);

        fclose(fp);
        return webpage;
    } else {
        return NULL;
    }
}