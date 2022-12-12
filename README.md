# CS50 Tiny Search Engine
## Design

A Tiny Search Engine (TSE) that crawls websites, indexes their content, and finally allows users to query for content.

Our Tiny Search Engine design consists of three subsystems:

1. The **crawler** crawls a website and retrieves webpages starting with a specified URL.
   It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number of hops from the seed URL, and avoiding visiting any given URL more than once.
   It saves the pages, and the URL and depth for each, in files.
   When the crawler process is complete, the indexing of the collected documents can begin.

2. The **indexer** extracts all the keywords for each stored webpage and creates a lookup table that maps each word found to all the documents (webpages) where the word was found.
   It saves this table in a file.

3. The **query engine** responds to requests (queries) from users.
   The *query processor module* loads the index file and searches for pages that include the search keywords.
   Because there may be many hits, we need a *ranking module* to rank the results (e.g., high to low number of instances of a keyword on a page).

Each subsystem is a standalone program executed from the command line, but they inter-connect through files in the file system. 

## Organization

Our TSE comprises six subdirectories:

* **libcs50** - a library of code provided by professor
* **common** - a library of code used by multiple modules
* **crawler** - the crawler (lab 4)
* **indexer** - the indexer (lab 5)
* **querier** - the querier (lab 6)
* **data** - with subdirectories where the crawler and indexer can write files, and the querier can read files.

```
.
|-- .gitignore
|-- Makefile
|-- README.md
|-- common
|   |-- Makefile
|   |-- index.c
|   |-- index.h
|   |-- pagedir.c
|   |-- pagedir.h
|   |-- word.c
|   `-- word.h
|-- crawler
|   |-- .gitignore
|   |-- DESIGN.md
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- REQUIREMENTS.md
|   |-- crawler.c
|   |-- testing.out
|   |-- testing.sh
|   `-- valgrind.sh
|-- data
|-- indexer
|   |-- .gitignore
|   |-- DESIGN.md
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- REQUIREMENTS.md
|   |-- indexer.c
|   |-- indextest.c
|   |-- testing.out
|   `-- testing.sh
|-- libcs50
|   |-- Makefile
|   |-- README.md
|   |-- bag.c
|   |-- bag.h
|   |-- counters.c
|   |-- counters.h
|   |-- file.c
|   |-- file.h
|   |-- hash.c
|   |-- hash.h
|   |-- hashtable.c
|   |-- hashtable.h
|   |-- libcs50-given.a
|   |-- mem.c
|   |-- mem.h
|   |-- set.c
|   |-- set.h
|   |-- webpage.c
|   `-- webpage.h
`-- querier
    |-- .gitignore
    |-- DESIGN.md
    |-- IMPLEMENTATION.md
    |-- Makefile
    |-- README.md
    |-- REQUIREMENTS.md
    |-- fuzzquery.c
    |-- querier.c
|   |-- testing.out
    |-- testing.sh
    `-- testing.txt
```

Our crawler, indexer, and querier each consist of just one `.c` file.
They share some common code, which we keep in the `common` directory:

* **index** - a suite of functions that implement the "index" data structure; this module includes functions to write an index to a file (used by indexer) and read an index from a file (used by querier).
* **pagedir** - a suite of functions to help the crawler write pages to the pageDirectory and help the indexer read them back in
* **word** - a function `normalizeWord` used by both the indexer and the querier.

Each of the program directories (crawler, indexer, querier) include a few files related to testing, as well.

## Note

See [Lab 4 (crawler)](https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab4/), [Lab 5 (indexer)](https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab5/), and [Lab 6 (querier)](https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab6/) for detailed project description.
