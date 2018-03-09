# Spell Checker in C

## What is it?
A program that spell-checks a .txt file against a provided dictionary text file.

## How was it built?
The whole program is written in C.

I use a trie data structure to check the spelling of each word in the text file against the specified dictionary. Each word from the dictionary is loaded into the trie structure, with one letter per node. Each word from the .txt file is then checked against this trie structure. If a complete words exists in the trie structure (`currentPtr->isWord == true`), then the program moves onto the next. If the word is not marked as a word in the trie structure, then the program records it as mispelled and will be printed as output.

The program outputs several benchmarks: including the mispellings, the number of words in the text and dictionary, the time to load the dictionary into the trie, the time to check the words against the trie, the time to unload the memory allocated for the trie, and the total time to perform all operations.

The program was built for Harvard's CS50 course. The course provided a codebase with missing functionality (see below). 

## What were the parameters of the problem?
*Paraphrased from the problem set's prompt*:

"Implement a program that spell-checks a file, per the below."

"In speller.c, we’ve put together a program that’s designed to spell-check a file after loading a dictionary of words from disk into memory. Unfortunately, we didn’t quite get around to implementing the loading part. Or the checking part. Both (and a bit more) we leave to you!"

In the dictionary.c file, "we’ve implemented...four functions, but only barely, just enough for this code to compile. Your job, ultimately, is to re-implement those functions as cleverly as possible so that this spell checker works as advertised."

See more in the [official docs](https://docs.cs50.net/problems/speller/speller.html).

