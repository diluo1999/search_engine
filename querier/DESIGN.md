# CS50 TSE Querier
## Design Specification

According to the [Querier Requirements Spec](REQUIREMENTS.md), the TSE *querier* is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### User interface

The querier's interface with the user is on the command-line; it must always have two arguments to initialize the querier.

```
querier pageDirectory indexFilename
```

For example, if `letters-depth-0` is a pageDirectory in `../data` and `letters-index-0` is the corresponding index file produced by the Indexer, 

``` bash
$ querier ../data/letters-depth-0 ../data/letters-index-0
```

After initializing the querier, the querier reads search queries from stdin, one query per line, until EOF. The expected queries are words connected with `and` and `or`.

For example, if the user wants to search for documents that have both `Computer` and `Science` (`Query?` is the instruction message from querier),

``` bash
Query? Computer Science
```

The querier prints a parsed version of the input query and then prints the number of documents found that satisfies the query and their information:

``` bash
Query computer science
Matches 1 documents (ranked):
score   2 doc  1: http://cs50tse.cs.dartmouth.edu/tse/toscrape/...
score   1 doc  2: http://cs50tse.cs.dartmouth.edu/tse/toscrape/...
```

### Inputs and outputs

**Input**: the querier opens the index files indicated by the `indexFilename` parameter.

The querier also gets URLs of webpages that satisfy the query from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID (as described in the Requirements).

**Output**: parsed query and number of matched documents are printed first. Then the score, doc ID, and URL of each webpage are printed.

### Functional decomposition into modules
We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules;
 2. *parseQuery*, which parses the query from stdin;
 2. *processQuery*, which processes the query and returns an accordingly counter set.
 3. *MERGE*, which pushes temporary result andSequence to final result orSequence and also resets andSequence.
 3. *UNION*, which unions orSequence and otherSequence by adding counts for matching keys and stores results in orSequence
 4. *unionFunction*, which is a helper function of UNION to update counters
 3. *INTERSECT*, which intersects orSequence and otherSequence by taking of counts for matching keys and stores results in orSequence.
 4. *intersectFunction*, which is a helper function of INTERSECT.
 3. *FINDMAX*, which finds the document that has the highest count.
 4. *findmaxFunction*, which is a helper function of FINDMAX.
 3. *counterFunction*, which is a helper function to count the number of doc that has count > 0.
 3. *prompt*, which prints a prompt iff stdin is a tty (terminal)
 3. *min*, which compares two number integer.

And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 1. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    while the user doesn't type ctrl+D
        print "query?" and read a line from stdin
        call parseQuery with stdin to get clean and validated query
        call processQuery with what parseQuery returns and the index module to get query results
        use iterate functions to rank the results and print them
        free memory

where *parseQuery:*
    
    verify the query string contains only letters and spaces
    tokenize the query string and build an array of words (tokens)
    print the clean query

where *processQuery:* (v = union, ^ = intersection)(credits to [CS50 Day23 Slides](https://www.cs.dartmouth.edu/~tjp/cs50/slides/Day23.pdf))

    counters_t *result = NULL
    counters_t *temp = NULL
    
    Read query one word at a time
        If read a word (not AND or OR)
            find counters for this word in index (index_find(index, word))
            if temp == NULL
                temp = counters for word
            else
            temp = temp ^ counters for word
        else if read OR
            result = result v temp
            temp = NULL
        else if read AND
            continue to next word
    Return result v temp

Other functions are illustrated in implementation specs.

### Major data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 
Two structs were used, which are struct twocounters that stores two pointers to or sequence counters and other sequence counters; struct maxDoc that stores two int pointers indicating the document with the largest count.
