#!/bin/bash
#
# testing.sh: test querier
#
# Input: none
# Output: result of testing cases written in testing.out
# 
# usage: bash -v testing.sh >& testing.out
#
# Di Luo, Oct 30 2022
# CS 50, Fall 2022

# variables
LOC="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output"
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all"

# Test querier with various invalid arguments

# Test 1: no arguments

# Expect:
# usage: ./querier pageDirectory indexFilename\n1
# 1

./querier
echo $?


# Test 2: one argument
# Expect:
# usage: ./querier pageDirectory indexFilename\n1
# 1

./querier $LOC/letters-depth-0
echo $?
 

# Test 3: three or more arguments

# Expect:
# usage: ./querier pageDirectory indexFilename\n1
# 1

./querier $LOC/letters-depth-0 $LOC/letters-index-0/ $LOC/letters-index-1/
echo $?


# Test 4: invalid pageDirectory (non-existent path)

# Expect: 
# invalid pageDirectory: /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth
# 2

./querier $LOC/letters-depth $LOC/letters-index-0
echo $?


# Test 5: invalid pageDirectory (not a crawler directory)

# Expect:
# invalid pageDirectory: ../data/invalid_letters
# 2

./querier $LOC $LOC/letters-index-0
echo $?


# Test 6: invalid indexFile (non-existent path)

# Expect:
# Cannot open file /letters-readonly/letters-index-0 in reading mode
# 3

./querier $LOC/letters-depth-0 $LOC/letters-index-7
echo $?


# Test 7: invalid queries with syntax errors

# testquery:
# and
# or
# and earth
# or earth
# planet earth or
# planet earth and
# planet earth and or science
# planet earth and and science
# planet earth or and science
# Warning!
# (The Lunar Chronicles #4.5)

./querier $LOC/letters-depth-0 $LOC/letters-index-0 < testquery


# Test 8: test with example output (see https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab6/querier-examples)

./querier $LOC/toscrape-depth-1 $LOC/toscrape-index-1 < testquery2


# Test 9: test with example output and valgrind

$VALGRIND ./querier $LOC/toscrape-depth-1 $LOC/toscrape-index-1 < testquery2

# Test 10: fuzz testing