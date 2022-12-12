/**
 * indextest.c - test program for CS50 'index' module
 * 
 * usage: ./indextest oldIndexFilename newIndexFilename
 * 
 * This program is a unit test for the index module
 * 
 * Di Luo
 * CS 50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"

/**************** main() ****************/
int main(const int argc, char* argv[]) {

    char* program = argv[0];
    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    if (argc != 3) {
        fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", program);
        return 1;
    }

    index_t* index = index_load(oldIndexFilename);
    if (index == NULL) {
        fprintf(stderr, "File %s is empty\n", oldIndexFilename);
        return 3;
    }

    index_save(index, newIndexFilename);
    index_delete(index);

    return 0;
}