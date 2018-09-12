/**
 * File: htable.c
 * @author: Vivian Breda, Josh King, Abinaya Saravanapavan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <ctype.h>
#include "mylib.h"

/**
 * Function: emalloc
 * Purpose: allocates a block of memory.
 *
 * @param s the size (bytes) of memory required for allocation.
 * @return the memory address where it has been allocated.
 */
void *emalloc(size_t s){
    void *result = malloc(s);
    if(NULL == result){
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Function: erealloc
 * Purpose: reallocates a block of memory.
 *
 * @param p is a void pointer to reallocate memory.
 * @param s the size (bytes) of memory required for reallocation.
 * @return the memory address where it has been reallocated.
 */
void *erealloc(void *p, size_t s){
    void *result = realloc(p, s);
    if (NULL == result){
        fprintf(stderr, "Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Function: getword
 * Purpose: reads in a word from the given stream, one at a time until the given * is reached or EOF  is returned. 
 *
 * @param s the char pointer to the array s.
 * @param limit the array size of s.
 * @param stream the source of the input.
 * @return -1 when end of file reached, or the word that has been read in.  
 */
int getword(char *s, int limit, FILE *stream)
{
    int c;
    char *w = s;
    assert(limit > 0 && s!= NULL && stream != NULL);

    while (!isalnum(c = getc(stream)) && EOF != c)
        ;
    if (EOF == c){
        return EOF;
    } else if (--limit > 0){
        *w++ = tolower(c);
    }
    while (--limit > 0){
        if (isalnum(c = getc(stream))){
            *w++ = tolower(c);
        } else if ('\'' == c){
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w-s;
}
