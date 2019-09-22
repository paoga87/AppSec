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

    char c;
    int strindex = 0;
    int read_count = 0;
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
            if (strindex <= LENGTH) // Avoid buffer overflow
                buf[strindex] = c;

        strindex++;
        read_count++;
    }
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    for (int i = 0; i < HASH_SIZE; i++) // Set hashtable to null
        hashtable[i] = NULL;

    FILE* wordlist;
    wordlist = fopen(dictionary_file, "r"); //Try to open wordlist.txt
    if (wordlist == NULL) {
        //printf("%s", "Failed to open dictionary file\n");
        return false;
    }
    int counter = 0;
    int bucket = 0;
    node* n = malloc(sizeof(node));
    n->next = NULL;
    while (read_word(wordlist, n->word) != EOF) {
        if (n->word == NULL) {
            //printf("%s\n", "NULL word detected.");
            continue;
        }
        counter++;
        bucket = hash_function(n->word);
        //printf("%s: %s %s: %d\n", "Loaded word", n->word, "bucket", bucket);
        if (hashtable[bucket] != NULL) {
            n->next=hashtable[bucket];
            hashtable[bucket]=n;
        } else {
            hashtable[bucket]=n;
        }
        n = malloc(sizeof(node));
        n->next = NULL;
}

    //printf ("%s%d\n", "Loaded dictionary word count: ", count);
    fclose(wordlist);
    return true;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
    
    //Lower the string
    int word_len = strlen(word);
    char lower_word[word_len];
    for(int i = 0; i<word_len; i++)
        lower_word[i] = tolower(word[i]);
    lower_word[word_len] = '\0';

    // Search linked list bucket for word
    int bucket = hash_function(lower_word);
    node* list_item = hashtable[bucket];
    while (list_item != NULL) {
        if (strcmp(list_item->word, lower_word) == 0)
            return true;
        list_item = list_item->next;
    }
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

    int num_misspelled = 0;

    // Reading file one word at a time
    char word[LENGTH];
    while (read_word(fp, word) != EOF) {
        // Prepare word to check
        // printf("%s: %s\n", "Word to check: ", word);

        char final_word[LENGTH];


        // Inspect string l to r
        int word_len = strlen(word);
        char lr_stage_word[word_len];
        int j = 0;
        int alpha_flag1 = 0;
        for(int i = 0; i<word_len; i++) {
            if (!isalpha(word[i]) && alpha_flag1 == 0 )
                continue;
            else
                alpha_flag1 = 1;

            lr_stage_word[j++] = word[i];
        }

        lr_stage_word[j] = '\0';


        // Inspect string r to l
        int lr_stage_word_len = strlen(lr_stage_word);
        char rl_stage_word[lr_stage_word_len];
        int y = 0;
        int alpha_flag2 = 0;

        for (int x = lr_stage_word_len - 1; x>=0; x--) {

            if (!isalpha(lr_stage_word[x]) && alpha_flag2 == 0 )
                continue;
            else
                alpha_flag2 = 1;

            rl_stage_word[y++] = lr_stage_word[x];
        }

        rl_stage_word[y] = '\0';

        // Reverse string
        int rl_stage_word_len = strlen(rl_stage_word);
        int b = 0 ;
        for ( int c = rl_stage_word_len -1; c>=0; c--){

            final_word[b++]=rl_stage_word[c];

        }

        final_word[b] = '\0';

        // printf("%s: %s\n", "Final word: ", final_word);

        if (check_word(final_word, hashtable) == true) {
        	// Final word is correctmsg
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

