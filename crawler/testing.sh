#!/bin/bash
#
# testing.sh: test crawler
#
# Input: none
# Output: result of testing cases written in testing.out
# 
# usage: bash -v testing.sh >& testing.out
#
# Di Luo, Oct 13 2022
# CS 50, Fall 2022

# TEST 1: wrong number of arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters
echo $?

# TEST 2: invalid URL
./crawler dartmouth.edu/tse/letters/index.html data/letters-3 0
echo $?

# TEST 3: external link
./crawler https://en.wikipedia.org/wiki/Wiki/index.html data/letters-3 0
echo $?

# TEST 4: invalid page directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 234234 0
echo $?

# TEST 5: invalid maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 11
echo $?

# TEST 6: letters at depths 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 0
echo $?

# TEST 7: letters at depths 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 1
echo $?

# TEST 8: letters at depths 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2
echo $?

# TEST 9: letters at depths 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2
echo $?

# TEST 10: toscrape at depths 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 0
echo $?

# TEST 11: toscrape at depths 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia 0
echo $?
