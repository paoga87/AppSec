#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICTIONARY "test_wordlist.txt"

START_TEST(test_check_word_normal) {
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* incorrect_word = "Caoimhín";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(incorrect_word, hashtable));
} END_TEST

START_TEST(test_check_word_buffer_overflow) {
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char incorrect_word[500000];
    for(int i = 0; i < 499999; i++)
        incorrect_word[i] = 'A';
    incorrect_word[499999] = 0;
    ck_assert(!check_word(incorrect_word, hashtable));
} END_TEST

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICTIONARY, hashtable));
    const char *word1 = "test";
    const char *word2 = "Athens";
    int bucket1 = hash_function("test");
    int bucket2 = hash_function("Athens");
    ck_assert_msg(strcmp(hashtable[bucket1]->word, word1) == 0);
    ck_assert_msg(strcmp(hashtable[bucket2]->word, word2) == 0);
}
END_TEST

START_TEST(test_check_word_normal2)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(TESTDICTIONARY, hashtable);
    const char *correct_word = "empirist's";
    const char *punct_word1 = "empirist.s";
    const char *punct_word2 = "?empirist";
    const char *punct_word3 = "empirist?";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punct_word1, hashtable));
    ck_assert(!check_word(punct_word2, hashtable));
    ck_assert(!check_word(punct_word3, hashtable));
}
END_TEST

Suite *check_word_suite(void) {
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_word_normal2);
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int main(void) {
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
