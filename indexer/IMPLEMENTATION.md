# CS50 TSE Indexer
## Implementation Specification

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

## Data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID.
*index* starts empty.
The size of *index* is impossible to determine in advance, so we use 500.
(From design spec)

## Control flow

The Indexer is implemented in one file `inderer.c`, with three functions. (Specs below are from [Cohesion and coupling activity solution](https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes21/activity_solution))

### 1. main

Parse the command line and validate parameters, build the index in memory by processing each webpage in pageDirectory, save index to file with indexFilename, and clean up data structures.

Pseudocode:

    parse the command line, validate parameters (pageDirectory and indexFilename)
    build the index in memory by processing each file (webpage) in pageDirectory
        index_t *index = indexBuild(pageDirectory);
    save index to file with indexFilename
        index_save(file, index);

    clean up data structures
        index_delete();

### 2. indexBuild

Loop over all webpages stored by crawler in pageDirectory, index those pages by looking for each word on each page, make entry in index (hashtable) for each word.

Pseudocode:

    create new index with index_new(number_slots);
    set docID =1;
    load webpage with webpage_t page = pagedir_load(pageDirectory, docID)
    while page not NULL
        index_page(index, page, docID) //make entry for each word in this page
        webpage_delete(page)
        docID++
        page = pagedir_load(pageDirectory, docID)
    return index

### 3. indexPage

Find words on a single page, create a counter for each word, make entry into index (hashtable).

Pseudocode: (different from pseudocode provided in the instruction)

    while (word == webpage_getNextWord()) not NULL
        if word length > 2
            normalizeWord(word)
            get counters for this word from index with index_find(index, word)
            if word not in index
                create new counters with counters_new()
                insert empty counters into index with index_insert(index, word, counters)
            increment word count with counters_add(counters, docID)
        free(word)

## Other modules

### pagedir

We created a re-usable module `pagedir.c` in Lab 4: Crawler to provide functions `pagedir_init`(to mark a directory as a Crawler directory) and `pagedir_save`(to save a page into a file in that directory). See implementation spec of Lab 4 for details. 
In Lab 5, we add `pagedir_validate`(to verify whether a directory is indeed a Crawler-produced directory) and `pagedir_load`(to load a page from a file in that directory).
According to requirements spec, pageDirectory has files named 1, 2, 3, …, without gaps and the content of files in pageDirectory follow the format as defined in the specs. Thus we don't extensively check errors.

Pseudocode for `pagedir_validate`:

    if the given pageDirectory is not NULL
        construct the pathname for the .crawler file in that directory
        open the file for reading; on error, return false
        close the file
        free the pathname buffer
        return true
    else
        return false

Pseudocode for `pagedir_load`:

    safety check on pageDirectory and docID
    construct the pathname for the page with docID being filename in that directory
    open fp on the constructed pathname for reading(with safety check that it opened)
    initialize pointers to URL, int storing depth, and pointer to HTML
    call file_readLine() twice to store URL in string buffer and depth in int buffer
    call file_readFile() to store HTML in string buffer
    create new webpage with buffers as parameters
    close fp

### index

We create re-usable module `index.c` providing the data structure to represent the in-memory index, and functions to read (index_) and write (index_save) index files.
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Pseudocode for `index_X` that call corresponding hashtable_X functions:

    return (index_t *) hashtable_X(parameters);

Pseudocode for `index_save`:

    safety checks on index and indexFilename
    open fp on indexFilename for writing (with safety check that it opened)
    iterate over hashtable with hashtable_iterate(index, fp, functionToPrintKey) to save the contents in fp
    close fp
   
    functionToPrintKey
        print key
        iterate over each counter with counters_iterate(counter, fp, functionToPrintCounter)
            
    functionToPrintCounter
        print count

Pseudocode for `index_load`:

    safety checks on indexFilename
    open fp with indexFilename for reading (with safety check that it opened)
    check for empty file (can use file_numlines(fp) to get word count because each word is on one line)
        print error message and return NULL if file contains no words
    create new index with index_new (how many slots? wordcount/2+1?)
    read each word in file with file_readWord(fp) 
        create new counter for word with counters_new()
        expect one or more docID/count pairs on line following word
        while line has docID/count pair
            increment counters with counters_set(counters, docID, count)
        //counters now contains one element for each docID
        add counters to hashtable with hashtable_insert(index, word, counters)
        free(word)
    close(fp)
    return index

### word

We create re-usable module `word.c` that takes a string as input and returns a normalized version of the string.

Pseudocode for `normalizeWord`:

    safety check on input word
    create a buffer for the normalized word
    for each char in the string until the null charactor
        copy from the input word to the buffer
    return the buffer

### libc50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so our crawler need not implement that part of the spec.
(From Lab 4 implementation spec)

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `crawler.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static index_t *indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```


### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior toeach function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t *pagedir_load(const char* pageDirectory, const int docID);
```


### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior toeach function's declaration in `index.h` and is not repeated here.

```c
index_t *index_new(int number_slots);
void index_save(index_t *index, const char *indexFilename );
index_t *index_load(const char *indexFilename);
void index_delete(index_t* index, void (*itemdelete)(void* item));
```


### word

Detailed descriptions of each function's interface is provided as a paragraph comment prior toeach function's declaration in `word.h` and is not repeated here.

```c
char *normalizeWord(const char* word);
```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example,  

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are three units, including indexer, index, pagedir, and word.

The indexer represents the whole system and is covered below.

The index unit is tested by the indextest program, which reads an index file into the internal index data structure, writes the index out to a new index file, and checks to see if the results are the same (we'll need to read an index for the querier). Use the command:
`make unittest`

The pagedir unit and the word unit are tiny; they could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe their behaviors during the system test.

From lab 5 instruction:
It can be tricky to compare two index files for equivalence - because the lines of an index file can be in any order, and the docIDs within a line can be in any order - so a simple run of `diff` won't always be sufficient.
Try using our `indexcmp` program; you can run it directly from the shared copy we installed:

```bash
$ {{site.filebase}}/workspace/tse/indexcmp
usage: {{site.filebase}}/workspace/tse/indexcmp indexFilenameA indexFilenameB
```

It takes two arguments, each a pathname to an index file; it will print out any apparent differences.

### Regression testing

In the process of implementation, command below can be used for regression testing after compilation:
`myvalgrind ./indexer /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-0 ../data/letters-index-0`

Logging function used in Lab 4 can be modified and used.

### Integration/system testing

Following the testing protocol mentioned in design spec, implement testing.sh that systematically test indexer. Use the command `make test` to run testing.sh and to store the results in testing.out.
