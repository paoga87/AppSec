#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int main(int argc, char* argv[]) {

    hashmap_t hashmap[HASH_SIZE];
    char * misspelled[1000];

    if ( argc < 3 || argc > 3) {
        printf ("%s", "Invalid arguments count\n");
        return -1;
    }

    if (load_dictionary(argv[2], hashmap) == false)
        return -2;

    // Read text file
    FILE* text_check;
    text_check = fopen(argv[1], "r");
    if (text_check == NULL) {
        printf("%s", "Failed to open file\n");
        return -3;
    }

    int mispelled_word_count = check_words(text_check, hashmap, misspelled);
    printf("%s: %d\n", "Misspelled word count: ", mispelled_word_count);
    for (int i = 0; i < mispelled_word_count; i++) {
        printf ("%s: %s\n", "Misspelled word: ", misspelled[i]);
    }
    return  0;
}