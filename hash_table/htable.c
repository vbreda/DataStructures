#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"

struct htablerec{
    int capacity;
    int num_keys;
    int *frequencies;
    char **keys;
    int *stats;
    hashing_t method;
};

htable htable_new(int cap, hashing_t method){
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


    for (i = 0; i < result->capacity; i++){
        result->frequencies[i] = 0;
        result->keys[i] = NULL;
        result->stats[i] = 0;
    }
    
    return result;
}

void htable_free(htable h){
    int i;

    for (i = 0; i < h->capacity; i++){
        free(h->keys[i]);
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

static unsigned int htable_step(htable h, unsigned int i_key){
  
    if (h->method == LINEAR_P) {
        return 1;
    }

    if (h->method == DOUBLE_H) {
        return 1 + (i_key % (h->capacity - 1));
    }

    fprintf(stderr, "not a valid hash method");
    exit(EXIT_FAILURE);
}

int htable_insert(htable h, char *str){

    unsigned int hash = htable_word_to_int(str) % h->capacity;
    unsigned int step = htable_step(h, hash);
    int collisions;

    if (!h->keys[hash]){
        h->keys[hash] = malloc(strlen(str) +1  * sizeof(h->keys[hash][0]));
        strcpy(h->keys[hash], str);
        h->frequencies[hash]++;
        h->num_keys++;

        return 1;
        
    } else if (strcmp(h->keys[hash], str) == 0){
        h->frequencies[hash]++;

        return h->frequencies[hash];
        
    } else{
        collisions = 1;
        while(collisions <= h->capacity){
            hash += step;
            hash = (hash)%h->capacity;
            
            if (h->keys[hash] == NULL){
                h->keys[hash] =
                    malloc(strlen(str) +1  * sizeof(h->keys[hash][0]));
                strcpy(h->keys[hash], str);
                h->frequencies[hash]++;
                h->stats[h->num_keys] = collisions;
                h->num_keys++;

                return 1;
        
            } else if (strcmp(h->keys[hash], str) == 0){
                h->frequencies[hash]++;

                return h->frequencies[hash];
            } else{
                collisions++;
                h->stats[h->num_keys] = collisions;
            }
        }
        return 0;
    }

}

void htable_print(htable h, FILE *stream){

    int i;

    for (i = 0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            fprintf(stream, "%s  -- Freq: %d\n", h->keys[i], h->frequencies[i]);
        }
    }
    printf("Num keys: %d\n", h->num_keys);
}

void htable_print_entire_table(htable h, FILE *stream){

    int i;

    for (i = 0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            fprintf(stream, "%5d %5d %5d  %s\n", i, h->frequencies[i], h->stats[i], h->keys[i]);
        }
    }
}

int htable_search(htable h, char *str){

    int collisions = 0;
    unsigned int hash = htable_word_to_int(str) % h->capacity;
    unsigned int step = htable_step(h, hash);
    
    while (h->keys[hash] != NULL && (strcmp(h->keys[hash], str) != 0)
           && collisions < h->capacity){

        hash += step;
        collisions++;
    }
    if (collisions >= h->capacity){
        return 0;
    } else{
        return h->frequencies[hash];
    }
}
