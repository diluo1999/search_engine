/* 
 * pagedir.h - header file for CS50 'pagedir' module
 * 
 * A 'pagedir' module handles the pagesaver mentioned in the design 
 * (writing a page to the pageDirectory), and marking it as a 
 * Crawler-produced pageDirectory (as required in the spec). 
 * We chose to write this as a separate module, in ../common, 
 * to encapsulate all the knowledge about how to initialize and 
 * validate a pageDirectory, and how to write and read page files, 
 * in one place… anticipating future use by the Indexer and Querier. 
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"

/**************** pagedir_init() ****************/
/* construct the pathname for the .crawler file in that directory
open the file for writing; on error, return false.
close the file and return true. */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save() ****************/
/* construct the pathname for the page file in pageDirectory
open that file for writing
print the URL
print the depth
print the contents of the webpage
close the file */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate() ****************/
/**
 * Verify whether a directory is indeed a Crawler-produced directory.
 * 
 * Caller provides:
 *      valid string for pageDirectory.
 * We return:
 *      false if pageDirectory is not valid or pageDirectory is NULL,
 *      true iff pageDirectory is a valid Crawler-produced directory.
 * Notes:
 *      We check if pageDirectory has a .crawler file.
*/
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load() ****************/
/**
 * Load a page from a file in the specified directory.
 * 
 * Caller provides:
 *      valid string for pageDirectory,
 *      valid int for document ID
 * We return:
 *      a webpage,
 *      NULL if pageDirectory/docID is invalid,
 * Caller is responsible for:
 *      later calling webpage_delete.
*/
webpage_t *pagedir_load(const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H