#!/bin/bash
#
# valgrind.sh: test crawler with valgrind
#
# Input: none
# Output: result of testing cases written in testing.out
# 
# usage: bash -v valgrind.sh >& valgrind.out
#
# Di Luo, Oct 13 2022
# CS 50, Fall 2022

# TEST 1: 10 maxdepth on letters
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 10
