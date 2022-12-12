# Makefile for modules in common directory
# Lab 4: Create 'pagedir' module
# Lab 5: Modified 'pagedir' module, created 'index' module and 'word' module
# 
# Di Luo
# CS 50, Fall 2022

# object files, and the target library
OBJS = pagedir.o index.o word.o
L = ../libcs50
LIBS = 
LLIBS = $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

.PHONY: test valgrind clean

########### common ##################
common.a: $(OBJS) $(LLIBS)
	ar cr common.a $^

# pagedir source dependencies
pagedir.o: pagedir.h $L/webpage.h $L/file.h $L/mem.h

# index source dependencies
index.o: index.h $L/hashtable.h $L/counters.h $L/file.h $L/mem.h

# word source dependencies
word.o: word.h $L/mem.h

# clean up after our compilation
clean:
	rm -f core
	rm -f *.a *~ *.o *.out