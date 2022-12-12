# Makefile for 'crawler' module
#
# Di Luo
# CS 50, Fall 2022

C = ../common
L = ../libcs50
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = # -DAPPTEST # -DMEMTEST

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
CC = gcc
MAKE = make

.PHONY: all test valgrind clean

########### crawler ##################

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies
crawler.o: $L/webpage.h $C/pagedir.h
crawler.o: $L/mem.h $L/bag.h $L/hashtable.h 

# clean up after our compilation
clean:
	rm -f core
	rm -f crawler *~ *.o *.out
