## bstsort

This program creates a binary search tree in C, then, taking input from stdin or a file, reads lines and organizes each line in ASCII order using the tree. It then outputs each line to stdout or a file, with a count next to each line signifying how many times it was found in the input.

`bstsort` was created as part of an assignment and is meant to be used as an educational code reference, i.e. to see an example of how a **very** basic binary search tree works in C.

It is by no means the most optimal or efficient way to create a binary search tree structure; the code within is mostly aimed at being readable by beginners to give them a working example of basic concepts of C, such as Makefiles, recursion, pass-by-pointer and structs.

### Usage

`usage: bstsort [-c] [-o output_file] [input_file]`

### Arguments

- **-c**: sort lines, taking case into account (default behavior is case-insensitive.
- **-o**: output line to file instead of stdout. If output_file does not exist, it will be created.
- **input_file**: optional; if specified, lines will be read from file at input_file instead of stdin.

## Running

`bstsort` can be run by cloning this repository into a local folder and running `make` (most easily done on Linux). Make sure you have a C compiler installed already; `gcc` is a [good choice](https://gcc.gnu.org/). 

### Sample Files

A couple of test files are included for convenience. `testfile` is a list of 500 randomly generated words (using [randomlists.com](https://www.randomlists.com/random-words)) and `testfile_sentences` is a list of 100 randomly generated sentences (using [randomwordgenerator.com](https://randomwordgenerator.com/sentence.php)).

### Sample build and run:

```
git clone https://gitlab.com/mtaylor76/bstsort.git
cd bstsort
make
./bstsort testfile
```

### Sample output:

```
2 apple
1 banana
1 laptop
2 xylophone
1 zebra
```