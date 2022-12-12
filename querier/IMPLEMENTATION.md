# CS50 TSE Indexer
## Implementation Specification

## Control flow

The Querier is implemented in one file `querier.c`, with ? functions.

### 1. main

### 2. parseQuery

### 3. processQuery

(credits to and modified [CS50 Day23 Activity Solution](https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes23/activity-solution))

Pseudocode:

    PROCESS QUERY
    // Create two counters initially set to NULL:
    counters_t *andSequence // holds result of current AND operations
    counters_t *orSequence  // accumulates the final result combining OR sequences
    shortCircuit = false    // stop early if AND sequence will fail

    Loop over all words in query
        if word == OR 
            // orSequence holds final result, merge in andSequence seen so far
            // think of stepping back after previously stepping away to calculate AND
            MERGE(andSequence, orSequence) 
            shortCicuit = false  //reset shortCircuit
            continue
        if shortCircuit 
            // at least one word returned no results, so AND sequence will fail
            // skip the words until see OR and shortCircuit is reset
            continue 
        if word == AND
            continue to next word // same as if AND wasn't there
        else found non operator word 
            // think of stepping away to calculate AND sequence
            get pages for this word with match = index_find(word)
            if match == NULL  // no pages contain this query word
                shortCircuit = true // AND will fail for next word, so skip it and others until see OR
                if andSequence != NULL 
                    // because this word returned no pages, drop everything AND'ed so far
                    delete andSequence
                    andSequence = NULL
            else  // found counters for this word in index
                if andSequence == NULL // this is the first word of AND sequence
                    andSequence = new counters
                    UNION(andSequence, match)  //essentially copies match to andSequence
                else // not first word in AND sequence
                    INTERSECT(andSequence, match) //intersect this word's pages 

    MERGE(andSequence, orSequence)
    return orSequence

### 4. Subsequent functions in processQuery

Pseudocode:

    MERGE(andSequence, orSequence):
        // push temporary result andSequence to final result orSequence
        // also reset andSequence
        if andSequence != NULL
            if orSequence == NULL
                orSequence = new counters
            UNION(orSequence, andSequence) //union andSequence into orSequence
            delete andSequence and set to NULL

    UNION(orSequence, otherSequence)
        // union orSequence and otherSequence by adding counts for matching keys
        // store results in orSequence
        counters_iterate(otherSequence, orSequence, unionFunction)

    unionFunction(arg, key, count)
    // update arg counters 
    wordCount = counters_get(arg, key) // returns 0 if key not found
    counters_set(arg, key, count + wordCount)

    INTERSECT(orSequence, otherSequence)
        // intersect orSequence and otherSequence by taking of counts for matching keys
        // store results in orSequence
        struct twocounters args = {orSequence, otherSequence}
        counters_iterate(orSequence, args, intersectFunction)

    intersectFunction(arg, key, count)
        counters_set(arg->orSequence, key, min(count, counters_get(args->otherSequence, key)))

### 5. Functions of ranking system

## Function prototypes

### querier

```c
int main(const int argc, char* argv[]);
int fileno(FILE *stream);
static void prompt(void);
static void MERGE(counters_t** andSequence, counters_t** orSequence);
static void UNION(counters_t* orSequence, counters_t* otherSequence);
void unionFunction(void *arg, const int key, const int count);
static void INTERSECT(counters_t* orSequence, counters_t* otherSequence);
void intersectFunction(void *arg, const int key, const int count);
static char** parseQuery(char* rawQuery);
static counters_t* processQuery(char** parsedQuery, index_t* index);
void findmaxFunction(void *arg, const int key, const int count);
static void FINDMAX(counters_t* orSequence, int *key, int *count);
void counterFunction(void *arg, const int key, const int count);
static inline int min(const int a, const int b);
```

## Testing plan

### Unit testing

After finishing a functionality, a unit test was run with a portion of the tests in testing.sh.
For example, to test 

### Integration/system testing

1. Test `querier` with various invalid arguments.
    2. no arguments
    2. one argument
    2. three or more arguments
    2. invalid `pageDirectory` (non-existent path)
    2. invalid `pageDirectory` (not a crawler directory)
    2. invalid `indexFile` (non-existent path)
    2. invalid queries with syntax errors
1. Test with example output (see https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab6/querier-examples)
1. Test with example output and valgrind
