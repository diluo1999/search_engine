## Crawler specifications

In this lab you'll begin the Tiny Search Engine (TSE) by coding the *Crawler* according to these specifications:

* [Requirements Spec](REQUIREMENTS.md)
* [Design Spec](DESIGN.md)
* [Implementation Spec](IMPLEMENTATION.md)

Grading will focus on [CS50 coding style](https://www.cs.dartmouth.edu/~tjp/cs50/style.html) - including consistent formatting, selection of identifier names, and use of meaningful comments - in addition to correctness, testing, and documentation.

***Your C code must compile without producing any compiler warnings.***  You will lose points if the compiler produces warnings when using our CS50-standard compiler flags.

***If your submitted code fails to compile, or triggers a segmentation fault,*** you will fail some or all of our correctness tests.
Write defensive code: each function should check its pointer parameters for NULL, and take some appropriate (safe) action.
Write solid unit tests, test drivers, and use regression testing as your development proceeds.

If your submitted version has *known bugs*, that is, cases where it fails your own test cases, *and you describe those cases in your README file*, we will halve the number of points you lose for those cases.
In short, it is far better for you to demonstrate you *know* about the bug than to submit and hope we won't find it.

***Valgrind should report no memory errors or memory leaks, when crawler exits normally.***
You will lose points for memory errors and leaks reported by valgrind on our tests.

## Crawler preparation

1. [Accept the assignment](https://classroom.github.com/a/mnFHzfJo), and clone the resulting repo.
1. Decide whose implementation of the Lab 3 data structures you want to use (you can always change later).
   * If you want to use *your* implementation, copy your `counters.c`, `set.c`, `hashtable.c` into the `libcs50` subdirectory in your brand-new repository; run `make` in that directory to ensure everything works.
   * If you want to use *our* implementation, do nothing.

   Run `make` from the top most directory and you should see a shared library file called `libcs50/libcs50.a`.
1. Create subdirectories `crawler` and `common` under the `tse` directory.

**Warning:** do **not** modify *any* of the files we provide in the starter kit; if we need to update anything in the starter kit, your changes will be overwritten.

> Exception: In the top-level Makefile you may wish to comment-out some of the recursive calls to make; such changes may be overwritten if we ask you to update the Makefile, but you can easily re-add those comments.

## Crawler assignment

Write the first sub-system of the Tiny Search Engine: the *crawler*.
Your implementation must follow the [Requirements](REQUIREMENTS.md) and [Design](DESIGN.md) Specs, and should follow the [Implementation](IMPLEMENTATION.md) Spec.

**In the top directory,**

 1. Update the `README.md` file to add your name and GitHub username.
 2. Comment-out the commands for indexer and querier, so `make` and `make clean` work to build (or clean) the libraries and crawler.

**In the `crawler` directory,**

 1. Write a program `crawler.c` according to the three Specs.
 1. Write a `Makefile` that will, by default, build the `crawler` executable program.
 1. Add a `make clean` target that removes files produced by Make or your tests.
 1. Add a `make test` target that tests your crawler.
    Read [about testing](#testing-crawler) below.
 1. Save the output of your tests with `make test &> testing.out`.
 1. Add a `README.md` file to describe any assumptions you made while writing the crawler, any ways in which your implementation differs from the three Specs, or any ways in which you know your implementation fails to work.
 1. Write a `.gitignore` file telling Git to ignore any unnecessary files in this directory (anything not already covered by the top-level `.gitignore`).

**In the `common` directory,** assemble code that will eventually be shared by the crawler, indexer, and querier.
For now, that comprises code for initializing the "pageDirectory" and saving webpages there.
(You will find reason to expand this file, and this directory, when you write the other subsystems.)

 1. Create files `pagedir.c` and `pagedir.h` as described in the [Implementation spec](IMPLEMENTATION.md).
 1. Write a `Makefile` that will, by default, build the `common.a` library.
    It must also have a `clean` target that removes files produced by Make.
 1. Write a `README.md` file to describe any assumptions you made while writing the crawler, any ways in which your implementation differs from the three Specs, or any ways in which you know your implementation fails to work. This file may be short.
 1. Write a `.gitignore` file telling Git to ignore any unnecessary files in this directory (anything not already covered by the top-level `.gitignore`).

## Crawler submission

Add/commit all the code and ancillary files required to build and test your solution; at a minimum your **crawler** directory should include the following files:
`.gitignore README.md Makefile crawler.c testing.sh testing.out`
and your **common** directory should contain the following files:
`Makefile pagedir.h pagedir.c`

*Do not commit any data files produced by the crawler, any binary/object files produced by the compiler, backup files, core dumps, etc.*

If you finish Lab 4 early, we encourage you to begin work on Lab 5 or Lab 6.
Your Lab 4 submission may contain a partly-completed indexer or querier; the graders will ignore those files, but must be able to build your crawler *from the top-level directory* without compilation errors and test your crawler without run-time errors.  You might consider using a separate git branch, the graders will use `main` branch.

To submit, read the [Lab submission instructions](https://www.cs.dartmouth.edu/~tjp/cs50/git/submit).


## libcs50

We provide several modules in the `libcs50` directory, which compiles to a library `libcs50.a` you can link with your crawler.
*You shall not change any of our code,*  but you may drop in your `set.c`, `counters.c`, `hashtable.c` files from Lab 3 if you wish.
The top-level Makefile auto-detects the presence of your `set.c` and will build the library with your implementation... or if it is absent, will simply use a pre-compiled library we provide.


**Pay close attention to the `webpage` module,**
especially the memory contract implemented by `webpage_new` and `webpage_delete`.

### Memory allocation

You may find the `mem` module useful.
Its use is optional, but it is ready to use as part of library `libcs50.a`.

In our Lab3 we tried to recover gracefully from memory-allocation failures.
In the TSE application programs, we'll be more lazy: on NULL return from malloc/calloc you can print an error to stderr and exit with non-zero exit status.
(You may find the `mem_assert()` family of functions to be useful here.)
In such cases, it is ok if valgrind reports your program as having leaks.

### Hashtable of URL

Our design calls for use of a Hashtable to store the URLs already seen.
Our Hashtable implementation stores a `void* item` with each key... but if you're just storing URLs (as keys), what item do you store?
Our hashtable disallows NULL items.
Suggestion: just pass a constant string as the item; even `""` will do.

### Testing Crawler

The [Implementation Spec](IMPLEMENTATION.md) instructs you to write a bash script `testing.sh` to test your crawler, and invoke it from the `make test` rule in the Makefile.
Sample output from our crawler can be found in `{{tse-dir}}/output`;
keep in mind that our crawler may process URLs in a different order, so your directory may not be identical to ours.

**Note:** the full suite of tests described in the [Implementation Spec](IMPLEMENTATION.md) can take more than an hour to run.
Although we encourage you to test your crawler using a variety of tests, your `testing.out` may be smaller, e.g., (`letters` at depths 0,10, `toscrape` at depths 0,1, `wikipedia` at depths 0,1).

### Use valgrind and gdb

We've provided information about [gdb](https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes14/) and [valgrind](https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes15/); make use of them.

## Summary

What are some of the key ideas in the crawler design?

First, it ***separates application-specific logic from general-purpose utility modules.***
By leveraging general-purpose modules from Lab 3, coding the crawler-specific logic is much cleaner and simpler than if it were woven in with the data-structure code.

Code that weaves the "business logic" of an application into the details of data structures is likely more complicated, more buggy, harder to debug, harder to test, harder to maintain, and much harder to reuse and extend.

Second, we note ***good data-structure design is key to successful projects.***
The design of the crawler remains simple because we chose two data structures – a *bag* for the set of pages yet to crawl, and a *hashtable* for the set of URLs already seen – that support the overall control flow.

For the TSE we've anticipated many design and implementation decisions for you – based on years of experience with this project.
In a real project, however, developers often find a need to go back and refine early modules to provide different functionality, or to factor out code that needs to be used by multiple components.
(As did we, when we first wrote this TSE implementation!)
It's hard to have perfect foresight.

[Pragmatic Programmer Tip](https://pragprog.com/tips/):

> **There are no final decisions:**
>  No decision is cast in stone.
> Instead, consider each as being written in the sand at the beach, and plan for change.

The software development process is precisely that: a process.
It is not a programmer on a one-way street, but more like a chef crafting a new dish.
You have a plan that everyone agrees on, you begin to execute on that plan, and sometimes conditions change and unexpected interactions arise.
You have to handle them and and still get to the goal.
