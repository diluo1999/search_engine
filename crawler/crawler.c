/* 
 * crawler.c - CS50 'crawler' module
 *
 * A 'crawler' crawls a website and retrieves webpages starting 
 * with a specified URL. It parses the initial webpage, extracts 
 * any embedded URLs and retrieves those pages, and crawls 
 * the pages found at those URLs, but limiting itself to some 
 * threshold number of hops from the seed URL (the ‘depth’), 
 * and avoiding visiting any given URL more than once. It 
 * saves the pages, and the URL and depth for each, in files. 
 * When the crawler process is complete, the indexing of the 
 * collected documents can begin. (CS50 Lab 4 Instruction)
 *
 * Di Luo
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../common/pagedir.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
/* declarations */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** logr ****************/
// log one word (1-9 chars) about a given url                                   
static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}

/**************** parseArgs ****************/
/* Given arguments from the command line, extract them 
into the function parameters; return only if successful. */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth) 
{
    char* program = argv[0];  // program name

	// check input arguments: # of arguments
	if (argc != 4) {
		fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", program);
		exit(1); 
	}

    // check input arguments: URL validity
	char* url = argv[1];

	if ((*seedURL = normalizeURL(url)) == NULL) {
		fprintf(stderr, "%s cannot be normalized\n", url);
        exit(2);
	}

    //make sure web page is on plank, not the Internet!
	if (!isInternalURL(url)) {
        mem_free(*seedURL);
		fprintf(stderr, "Web page is not internal: %s\n",url);
		exit(3);
	}

    // check input arguments: pageDirectory validity
    char* pd = argv[2];

    if (!pagedir_init(pd)) {
        fprintf(stderr, "Cannot initialize page directory: %s\n",pd);
		exit(4);
    }

    *pageDirectory = pd;

    // check input arguments: maxDepth validity
    int md = atoi(argv[3]);

    // check if argv[3] is a number
    // if (!isdigit(atoi(argv[3]))) {
    //     fprintf(stderr, "Need a integer in the range [0..10]: %s\n", md);
	// 	exit(5);
    // }
    // check if maxDepth is range [0..10]
    if (md < 0 || md > 10) {
        fprintf(stderr, "%d is not in the range [0..10]\n", md);
        exit(5);
    }

    *maxDepth = md;
}

/**************** crawl ****************/
/* Do the real work of crawling from seedURL 
to maxDepth and saving pages in pageDirectory */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) 
{
    // initialize the hashtable and add the seedURL
    hashtable_t* ht = hashtable_new(200);
    if (!hashtable_insert(ht, seedURL, "")) {
        fprintf(stderr, "Cannot insert seedURL into hashtable");
		exit(6);
    }

    // initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* bag = bag_new();
    webpage_t* web = webpage_new(seedURL, 0, NULL);
    bag_insert(bag, web);

    // initialize page directory
    if (!pagedir_init(pageDirectory)) {
        fprintf(stderr, "Cannot initialize the page directory: %s", pageDirectory);
		exit(7);
    }

    // define local variables
    webpage_t* web_popped = NULL;
    int documentID = 1;

    // while bag is not empty
    // pull a webpage from the bag
    while ((web_popped = bag_extract(bag)) != NULL) {
        // fetch the HTML for that webpage
        if (webpage_fetch(web_popped)) { 
             // if fetch was successful,
            logr("Fetched", webpage_getDepth(web_popped), webpage_getURL(web_popped));
            // save the webpage to pageDirectory
            pagedir_save(web_popped, pageDirectory, documentID);
            documentID++;
            // if the webpage is not at maxDepth
            if (webpage_getDepth(web_popped) != maxDepth) {
                // pageScan that HTML
                logr("Scanning", webpage_getDepth(web_popped), webpage_getURL(web_popped));
                pageScan(web_popped, bag, ht);
            }
            // delete that webpage
            webpage_delete(web_popped);
        }
    }
    // delete the hashtable
    hashtable_delete(ht, NULL);
    // delete the bag
    bag_delete(bag, NULL);
}

/**************** pageScan ****************/
/* This function implements the pagescanner mentioned 
in the design. Given a webpage, scan the given page to 
extract any links (URLs), ignoring non-internal URLs; 
for any URL not already seen before (i.e., not in the 
hashtable), add the URL to both the hashtable 
pages_seen and to the bag pages_to_crawl */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    char* url;
    // char* urlcopy1;
    char* urlcopy2;
    int pos = 0;
    webpage_t* web_temp;
    // while there is another URL in the page
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        logr("Found", webpage_getDepth(page), url);
        // if that URL is Internal,
        if (isInternalURL(url)) {
            // insert the webpage into the hashtable
            if (hashtable_insert(pagesSeen, url, "")) {
                // if that succeeded,
                // create a webpage_t for it
			    // insert the webpage into the bag
                urlcopy2 = mem_assert(malloc(strlen(url)+1), "url copy 2");
	            strcpy(urlcopy2, url);
                web_temp = webpage_new(urlcopy2, webpage_getDepth(page)+1, NULL);
                bag_insert(pagesToCrawl, web_temp);
                logr("Added", webpage_getDepth(page), url);
            } else {
                logr("IgnDupl", webpage_getDepth(page), url);
            }
        } else {
            logr("IgnExtrn", webpage_getDepth(page), url);
        }
        // free the URL
        mem_free(url);
    }
}

/**************** global functions ****************/
/* visible outside this file */

/**************** main ****************/
/* calls parseArgs and crawl, exits zero */
int main(const int argc, char* argv[]) 
{
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    return 0;
}