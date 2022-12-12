# Makefile for querier
# 
# Di Luo
# CS 50, Fall 2022

C = ../common
L = ../libcs50
LIBS = 
LLIBS = $C/common.a $L/libcs50.a
# for the given output files
LOC = /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output
# $^ refers to “the dependencies listed in this rule”
# $@ refers to “the target of this rule”

# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = # -DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$C -I$L
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean

########### querier ##################

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# querier source dependencies
querier.o: $C/index.h $C/word.h $C/pagedir.h $L/mem.h $L/file.h $L/counters.h $L/webpage.h

########### test ##################

test: querier testing.sh 
	bash -v testing.sh >& testing.out

########### clean ##################

# clean up after our compilation
clean:
	rm -f core
	rm -f querier *.a *~ *.o *.out