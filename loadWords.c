#include "data.h"
#include <stdio.h>
#include <string.h>

// Array to store up to 200 words, each with a maximum length of 19 characters
char words[200][20];

/**
 * Function: load_word_pool
 * ------------------------
 * Reads words from standard input and stores them in the words array.
 * The function will read up to 200 words, each with a maximum length of 19 characters.
 * Input is terminated when EOF is reached or when 200 words have been read.
 */
void load_word_pool() {
    int i = 0;
    // Read words from standard input until EOF or the array is full
    while (scanf("%19s", words[i]) == 1 && i < 200) {
        i++;
    }
}