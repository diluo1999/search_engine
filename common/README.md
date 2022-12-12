# Common directory

This directory will hold code used by multiple modules in the Tiny Search Engine (TSE).

## pagedir

A `pagedir` module handles the pagesaver mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec). We chose to write this as a separate module, in ../common, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place… anticipating future use by the Indexer and Querier.

See pagehir.h for implementation details.

## index

A `index` module is a wrapper around `hashtable` that focuses on storing `counters` as items in the `hashtable`’s set. It implements an abstract `index_t` type that represents an index in memory. In addition to implementing a hashtable of counters, `index` also provides functions to save an index to disk and to load it into memory from disk. The module is implemented in Lab 5 for `indexer` and the querier in Lab 6 will also use these functions.

See index.h for implementation details.

## word

A `word` module has one function normalizeWord() that normalizes the letters in the word to lower case. 

See word.h for implementation details.
