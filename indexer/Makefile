# Makefile for indexer and indextest
# 
# Di Luo
# CS 50, Fall 2022

C = ../common
L = ../libcs50
D = ../data
LIBS = 
LLIBS = $C/common.a $L/libcs50.a
# for the given output files
LOC = /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output
# for index unit test 
INDEXCMP = /thayerfs/courses/22fall/cosc050/workspace/tse/indexcmp
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

.PHONY: all unittest test valgrind clean

########### all ##################

# default
all: indexer indextest

########### indexer ##################

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies
indexer.o: $C/index.h $C/word.h $C/pagedir.h $L/mem.h $L/webpage.h

########### indextest ##################

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies
indextest.o: $C/index.h

########### unittest ##################

# unit test on index
unittest: indextest
	touch $D/letters-index-0
	./indextest $(LOC)/letters-index-0 $D/letters-index-0
	$(INDEXCMP) $(LOC)/letters-index-0 $D/letters-index-0
	touch $D/letters-index-1
	./indextest $(LOC)/letters-index-1 $D/letters-index-1
	$(INDEXCMP) $(LOC)/letters-index-1 $D/letters-index-1
	touch $D/letters-index-6
	./indextest $(LOC)/letters-index-6 $D/letters-index-6
	$(INDEXCMP) $(LOC)/letters-index-6 $D/letters-index-6
	touch $D/toscrape-index-1
	./indextest $(LOC)/toscrape-index-1 $D/toscrape-index-1
	$(INDEXCMP) $(LOC)/toscrape-index-1 $D/toscrape-index-1
	touch $D/wikipedia-index-1
	./indextest $(LOC)/wikipedia-index-1 $D/wikipedia-index-1
	$(INDEXCMP) $(LOC)/wikipedia-index-1 $D/wikipedia-index-1

########### test ##################

test: indexer testing.sh 
	bash -v testing.sh >& testing.out

########### clean ##################

# clean up after our compilation
clean:
	rm -f core
	rm -f indexer indextest *.a *~ *.o *.out