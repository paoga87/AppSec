/****************************************************************************
 * spell.c
 *
 * Application Security, Assignment 1
 *
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


extern int hash_function(const char* word);

int read_word(FILE* fp, char* buf) {

    //printf("%s\n", "Reading word");

    // Initialize character
    //            string index
    //            read count
    char c;
    int strindex = 0;
    int read_count = 0;

    // While we're reading the characters
    // Check for empty spaces or EOF
    while((c = fgetc(fp))) { 
        if(c == ' ' || c == '\n' || c == '\t' || c == EOF) { 
            if ( strindex <= LENGTH + 1 && strindex > 0) 
                buf[strindex] = '\0'; 
            if (c == EOF && strindex == 0)
                return EOF;
            else if ( strindex == 0)
                continue;
            else
                return strindex;
        }
        else 
            // Avoid buffer overflow
            if (strindex <= LENGTH)
                buf[strindex] = c;

        strindex++;
        read_count++;
    }
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    
    // Initializing all values in hash table to NULL
    for (int i = 0; i < HASH_SIZE; i++)
        hashtable[i] = NULL;

    // Open dictionary_file 
    FILE* wordlist;
    wordlist = fopen(dictionary_file, "r"); 

    // If dictionary_file is NULL, return false
    if (wordlist == NULL) {
        //printf("%s", "Failed to open dictionary file\n");
        return false;
    }

    int counter = 0;
    int bucket = 0;

    // Set hashmap_t new_node to a new node
    node* n = malloc(sizeof(node));
    
    // Set new_node->next to NULL
    n->next = NULL;

    // While word in dict_file is not EOF (end of file)
    while (read_word(wordlist, n->word) != EOF) {
        if (n->word == NULL) {
            //printf("%s\n", "NULL word detected.");
            continue;
        }

        counter++;
        // Set int bucket to hash_function(word)
        bucket = hash_function(n->word);
        

        // if hashtable[bucket] is NULL:
        //     Set hashtable[bucket] to new_node
        // else:
        //     Set new_node->next to hashtable[bucket]
        //     Set hashtable[bucket] to new_node

        if (hashtable[bucket] != NULL) {
            n->next=hashtable[bucket];
            hashtable[bucket] = n;
        } else {
            hashtable[bucket] = n;
        }

        n = malloc(sizeof(node));
        n->next = NULL;
    }

    //printf ("%s%d\n", "Loaded dictionary word count: ", count);
    // Close dictionary_file
    fclose(wordlist);
    return true;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
    
    //Lowercasing the string
    int word_length = strlen(word);
    char lowercase_word[word_length];
    for(int i = 0; i<word_length; i++)
        lowercase_word[i] = tolower(word[i]);
    lowercase_word[word_length] = '\0';

    // Search linked list bucket for word
    // Set int bucket to the output of hash_function(word)
    int bucket = hash_function(lowercase_word);

    //Set hashmap_t cursor equal to hashmap[bucket].
    node* list_item = hashtable[bucket];

    // While cursor is  not NULL:
    //   If lower_case(word) equals curosr->word:
    //       return True
    //   Set curosr to cursor->next
    // return False
    while (list_item != NULL) {
        if (strcmp(list_item->word, lowercase_word) == 0)
            return true;
        list_item = list_item->next;
    }
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

    // Set int num_misspelled to 0
    int num_misspelled = 0;

    // Reading file one word at a time
    char word[LENGTH];
    while (read_word(fp, word) != EOF) {

        // Prepare word to check
        // printf("%s: %s\n", "Word to check: ", word);
        char final_word[LENGTH];


        // Inspect string l to r
        int word_length = strlen(word);
        char lr_stage_word[word_length];
        int j = 0;
        int alpha_flag1 = 0;
        for(int i = 0; i<word_length; i++) {
            if (!isalpha(word[i]) && alpha_flag1 == 0 )
                continue;
            else
                alpha_flag1 = 1;

            lr_stage_word[j++] = word[i];
        }

        lr_stage_word[j] = '\0';


        // Inspect string r to l
        int lr_stage_word_length = strlen(lr_stage_word);
        char rl_stage_word[lr_stage_word_length];
        int y = 0;
        int alpha_flag2 = 0;

        for (int x = lr_stage_word_length - 1; x >= 0; x--) {

            if (!isalpha(lr_stage_word[x]) && alpha_flag2 == 0 )
                continue;
            else
                alpha_flag2 = 1;

            rl_stage_word[y++] = lr_stage_word[x];
        }

        rl_stage_word[y] = '\0';

        // Reverse string
        int rl_stage_word_length = strlen(rl_stage_word);
        int b = 0 ;
        for ( int c = rl_stage_word_length -1; c >= 0; c--){

            final_word[b++] = rl_stage_word[c];

        }

        final_word[b] = '\0';

        // printf("%s: %s\n", "Final word: ", final_word);

        if (check_word(final_word, hashtable) == true) {
        	// Final word is correct
            // printf("%s %s\n", final_word, "is correct");
        }
        else {

            misspelled[num_misspelled] = malloc(sizeof(char) * strlen(final_word));
            
            // Append word to misspelled
            strcpy(misspelled[num_misspelled], final_word);
            
            // Increment num_misspelled
            num_misspelled++;
        }

    }
    // Return num_misspelled
    return num_misspelled;
}

