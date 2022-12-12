#!/bin/bash
#
# testing.sh: test indexer
#
# Input: none
# Output: result of testing cases written in testing.out
# 
# usage: bash -v testing.sh >& testing.out
#
# Di Luo, Oct 17 2022
# CS 50, Fall 2022

# variables
LOC="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output"
D="../data"
INDEXCMP="/thayerfs/courses/22fall/cosc050/workspace/tse/indexcmp"
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all"


# Test indexer with various invalid arguments

# Test 1: no arguments
# Expect:
# usage: ./indexer pageDirectory indexFilename\n1
# 1
./indexer
echo $?

# Test 2: one argument
# Expect:
# usage: ./indexer pageDirectory indexFilename\n1
# 1
./indexer $LOC/letters-depth-0
echo $?
 
# Test 3: three or more arguments
# Expect:
# usage: ./indexer pageDirectory indexFilename\n1
# 1
touch $D/letters-index-0
touch $D/letters-index-1
./indexer $LOC/letters-depth-0 $D/letters-index-0/ $D/letters-index-1/
echo $?

# Test 4: invalid pageDirectory (non-existent path)
# Expect: 
# invalid pageDirectory: /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth
# 2
./indexer $LOC/letters-depth $D/letters-index-0
echo $?

# Test 5: invalid pageDirectory (not a crawler directory)
# Expect:
# invalid pageDirectory: ../data/invalid_letters
# 2
mkdir ../data/invalid_letters
./indexer $D/invalid_letters $D/letters-index-0
echo $?

# Test 6: invalid indexFile (non-existent path)
./indexer $LOC/letters-depth-0 $D/letters-index-10
echo $?

# Test 7: invalid indexFile (read-only directory)
mkdir $D/letters-readonly
touch $D/letters-readonly/letters-index-0
chmod a=r $D/letters-readonly
./indexer $LOC/letters-depth-0 $D/letters-readonly/letters-index-0
echo $?

# Test 8: invalid indexFile (existing, read-only file)
touch $D/letters-index-readonly
chmod -w $D/letters-index-readonly
./indexer $LOC/letters-depth-0 $D/letters-index-readonly
echo $?


# Run indexer on a variety of pageDirectories and use indextest as one means of validating the resulting index.

# Test 9: letters-depth-0
./indexer $LOC/letters-depth-0 $D/letters-index-0
$INDEXCMP $LOC/letters-index-0 $D/letters-index-0

# Test 10: letters-depth-1
./indexer $LOC/letters-depth-1 $D/letters-index-1
$INDEXCMP $LOC/letters-index-1 $D/letters-index-1

# Test 11: letters-depth-2
touch $D/letters-index-2
./indexer $LOC/letters-depth-2 $D/letters-index-2
$INDEXCMP $LOC/letters-index-2 $D/letters-index-2

# Test 12: letters-depth-3
touch $D/letters-index-3
./indexer $LOC/letters-depth-3 $D/letters-index-3
$INDEXCMP $LOC/letters-index-3 $D/letters-index-3

# Test 13: letters-depth-4
touch $D/letters-index-4
./indexer $LOC/letters-depth-4 $D/letters-index-4
$INDEXCMP $LOC/letters-index-4 $D/letters-index-4

# Test 14: letters-depth-5
touch $D/letters-index-5
./indexer $LOC/letters-depth-5 $D/letters-index-5
$INDEXCMP $LOC/letters-index-5 $D/letters-index-5

# Test 15: letters-depth-6
touch $D/letters-index-6
./indexer $LOC/letters-depth-6 $D/letters-index-6
$INDEXCMP $LOC/letters-index-6 $D/letters-index-6

# Test 16: toscrape-depth-0
touch $D/toscrape-index-0
./indexer $LOC/toscrape-depth-0 $D/toscrape-index-0
$INDEXCMP $LOC/toscrape-index-0 $D/toscrape-index-0

# Test 17: toscrape-depth-1
touch $D/toscrape-index-1
./indexer $LOC/toscrape-depth-1 $D/toscrape-index-1
$INDEXCMP $LOC/toscrape-index-1 $D/toscrape-index-1

# Test 18: wikipedia-depth-0
touch $D/wikipedia-index-0
./indexer $LOC/wikipedia-depth-0 $D/wikipedia-index-0
$INDEXCMP $LOC/wikipedia-index-0 $D/wikipedia-index-0

# Test 19: wikipedia-depth-1
touch $D/wikipedia-index-1
./indexer $LOC/wikipedia-depth-1 $D/wikipedia-index-1
$INDEXCMP $LOC/wikipedia-index-1 $D/wikipedia-index-1

# Run valgrind on both indexer and indextest to ensure no memory leaks or errors
# Test 21: valgrind indextest
$VALGRIND ./indextest $LOC/letters-index-6 $D/letters-index-6

# Test 21: valgrind letters-depth-6
$VALGRIND ./indexer $LOC/letters-depth-6 $D/letters-index-6

# Test 22: valgrind toscrape-depth-0
$VALGRIND ./indexer $LOC/toscrape-depth-0 $D/toscrape-index-0

# Test 23: valgrind wikipedia-depth-0
$VALGRIND ./indexer $LOC/toscrape-depth-0 $D/toscrape-index-0
