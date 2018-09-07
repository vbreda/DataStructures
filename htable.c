#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    int *frequencies;
    char **keys;
    int *stats;
    hashing_t method;
};

htable htable_new(int cap, hashing_t method) {
    int i;
    htable result = emalloc(sizeof *result);
    result->capacity = cap;
    result->num_keys = 0;
    result->method = method;
    result->frequencies =
        emalloc(result->capacity * sizeof result->frequencies[0]);
    result->keys =
        emalloc(result->capacity * sizeof result->keys[0]);
    result->stats = emalloc(result->capacity * sizeof result->stats[0]);


    for (i = 0; i < result->capacity; i++) {
        result->frequencies[i] = 0;
        result->keys[i] = NULL;
        result->stats[i] = 0;
    }
    
    return result;
}

void htable_free(htable h) {
    int i;

    for (i = 0; i < h->capacity; i++) {
        if (h->keys[i]){
            free(h->keys[i]);
        }
    }
    
    free(h->keys);
    free(h->frequencies);
    free(h->stats);
    free(h);
}

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;

    while (*word != '\0') {
        result = ((*word++) + 31 * result);
    }
    
    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
  
    if (h->method == LINEAR_P) {
        return 1;
    }

    if (h->method == DOUBLE_H) {
        return 1 + (i_key % (h->capacity - 1));
    }

    fprintf(stderr, "not a valid hash method");
    exit(EXIT_FAILURE);
}

int htable_insert(htable h, char *str) {

    unsigned int index, hash, i, position, collisions = 0;
    unsigned int step;

    index = htable_word_to_int(str);
    hash = index % h->capacity;
    step = htable_step(h, index);
    
    if (h->keys[hash] != NULL && strcmp(h->keys[hash], str) == 0) {
        h->frequencies[hash]++;
        return h->frequencies[hash];
    } else {
        position = hash;
        i = position;
        do {
            if (h->keys[i] == NULL){
                h->keys[i] = emalloc((strlen(str)+1) * sizeof (h->keys[i][0]));
                strcpy(h->keys[i], str);
                h->frequencies[i]++;
                h->num_keys++;
                h->stats[h->num_keys] = collisions;
                return 1;
            } else if (strcmp(h->keys[i], str) == 0) {
                collisions++;
                h->frequencies[i]++;
                return h->frequencies[i];
            }
            i = (i+step)%h->capacity;
            collisions++;
        } while (i != position);

        return 0;
    }
    return 0;
}

void htable_print(htable h, void f(int freq, char *str)) {

    int i;

    for (i = 0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            f(h->frequencies[i], h->keys[i]); 
        }
    }
}

void htable_print_entire_table(htable h, FILE *stream) {

    int i;

    fprintf(stream, "%5s %5s %6s  %s\n", "Pos", "Freq", "Stats", "Word");
    fprintf(stream, "%s\n", "----------------------------------------");
    
    for (i = 0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], h->keys[i]);
        } else {
            fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], "");
        }
    }
}

int compare(char *str1, char *str2) {
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int htable_search(htable h, char *str) {

    int collisions = 0;
    unsigned int index = htable_word_to_int(str);
    unsigned int hash = index % h->capacity;
    unsigned int step = htable_step(h, hash);

    while (h->keys[hash] != NULL &&
           compare(str, h->keys[hash]) != 0
           && collisions != h->capacity) {

        hash += step;
        hash %= h->capacity;
        collisions++;
    }
    
    if (collisions == h->capacity) {
        return 0;
    } else {
        return h->frequencies[hash];
    }
}

/**
 * Supplied code
 */

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", 
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}

