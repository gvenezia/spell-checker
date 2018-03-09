/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

// Create a trie to store and search the dictionary
typedef struct Trie {
    bool isWord;
    struct Trie *nextPtr[26];
} trie;

// Synonym for trie*
typedef trie *triePtr;

// Declare rootPtr and currentPtr as global variables for use in load, check, and unload.
triePtr rootPtr = NULL;
triePtr currentPtr = NULL;

// Declare the word counter to record words in load and report the number in size
int wordCount = 0;

// Function prototype
// Recursive function that closes out a whole trie
void closeTrie(triePtr cPtr);

/**
 * Returns true if word is in dictionary, else false.
 */
bool check(const char *word)
{
    // Search the dictionary (stored in memory with struct Trie)

    // Declare index and assign to 0
    int index = 0;

    // Make sure to start at the root of the trie
    currentPtr = rootPtr;

    for (int i = 0; i < strlen(word); i++)
    {
        // Get an index compatible with the trie, change char to int,
        // return false if character not in dictionary
        if ( isalpha(word[i]) ) {
            // Make sure all letters are lowercase
            if ( isupper(word[i]) ){
                index = word[i] - (int)'A';
            }
            else{
                index = word[i] - (int)'a';
            }
        }
        else if (word[i] == '\'') {
            index = 26;
        }
        else {
            return false;  // Character is not in the dictionary
        }

        // If the pointer is NULL then the word can't be in the dictionary, return false
        // In other words, if the current letter in the word doesn't correspond to a letter in
        // the dictionary trie, then the word is not in the dictionary
        if ( currentPtr->nextPtr[index] == NULL ) {
            return false;
        }
        else{
            // Move pointer to the next node
            currentPtr = currentPtr->nextPtr[index];
        }

    } // End for loop

    // If there is a word in the last child, then we have successfully checked for the word in the dictionary
    if (currentPtr->isWord == true){
        return true;
    }
    else // If there isn't a word in the last child, we've unsuccessfully checked
    {
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful, else false.
 */
bool load(const char *dictionary)
{

// Load into the memory with malloc
// (perhaps calloc would be better? see: https://stackoverflow.com/questions/1538420/difference-between-malloc-and-calloc )

    // Open the dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL){
        return false;
    }

    // Create a trie with dictionary entries
    // Declare newPtr (root and current pointers are global variables)
    triePtr newPtr = NULL;

    rootPtr = calloc(1, sizeof(struct Trie) + 8 );

    // Check for successful memory allocation and assign
    if (rootPtr != NULL) {
        rootPtr->isWord = false;
        // for (int j = 0; j <=26; j++){
        //     rootPtr->nextPtr[j] = NULL;
        // }
    } else {
        unload();
        fprintf(stderr, "Memory could not be allocated to load the dictionary.\n");
        return false;
    }

    // Declare index for nextPtr's within trie
    int index = 0;

    // Declare a char array to store the word from the dictionary file
    char fileWord[LENGTH + 2];

    // Get string with fgets
    while (fgets(fileWord, LENGTH + 2, dict) != NULL)
    {
        // Assign rootPtr to currentPtr
        currentPtr = rootPtr;

        // Iterate over each letter in string, traverse the tree. If node == NULL, create node
        for (int i = 0; i < strlen(fileWord); i++)
        {
            // Change char to int in order to assign to the array
            if ( isalpha(fileWord[i]) ) {
                index = fileWord[i] - (int)'a';
            }
            else if (fileWord[i] == '\'') {
                index = 26;
            }
            else // End of word
            {
                // Change the boolean value to true to indicate a complete word
                currentPtr->isWord = true;

                // Add to the wordCount for the dictionary (for use in size)
                wordCount++;
                break;
            }

            // Create a new node if the current character doesn't point to a trie node
            if ( currentPtr->nextPtr[index] == 0 )
            {
                // Allocate memory to the next node
                newPtr = calloc(1, sizeof(struct Trie) + 8 );

                // Check for successful memory allocation and assign members
                if (newPtr != NULL) {
                    newPtr->isWord = false;
                    // for (int j = 0; j <=26; j++){
                    //     newPtr->nextPtr[j] = NULL;
                    // }
                } else {
                    unload();
                    fprintf(stderr, "Memory could not be allocated to add a node to the trie.\n");
                    return false;
                }

                // Link the currentPtr to the newPtr
                currentPtr->nextPtr[index] = newPtr;

                // Reassign newPtr to NULL to prepare for next new node
                // (so it doesn't override the node just created )
                newPtr = NULL;
            } // End create new node condition

            // Move pointer to the next node
            currentPtr = currentPtr->nextPtr[index];

        } // End for loop

    } // End while loop getting successive strings

    // Close the dictionary
    fclose(dict);

    // Check for loaded dictionary
    return true;

} // End load function

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Probably need to traverse the tree and count the branches

    if (!&load)
    {
        fprintf(stderr, "Dictionary not loaded. (Problem encountered in size function)\n");
        return 0;
    }

    return wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // Check to make sure closeTrie cleared the trie
    if ( rootPtr != NULL ){
        // Call the recursive function to traverse all branches of the trie
        closeTrie(rootPtr);

        return true;
    }
    return false;
}

/**
 * Recursive function that closes out a whole trie
 */
void closeTrie(triePtr cPtr)
{
    // Iterate over the children pointers of tempPtr, recursively close all its non-NULL children
    for (int i = 0; i <= 26; i++)
    {
        if ( cPtr->nextPtr[i] != NULL ) {
            // If the child is not empty, then recursively close that child
            closeTrie(cPtr->nextPtr[i]);
        }
    }

    // All node children should be NULL
    // Empty contents
    cPtr->isWord = NULL;

    // And free the node
    free(cPtr);

} // End closeTrie
