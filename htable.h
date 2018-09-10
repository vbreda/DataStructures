/**
 * File: htable.h
 * @author Vivian Breda, Josh King, Abinaya Saravanapavan.
 */

#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

/**
 * Struct: htablerec
 * Purpose: defining a struct type of htablerec to structure the htable.
 */
typedef struct htablerec *htable;

typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;

/**
 * Prototypes
 * Purpose: specifies functions to be implemented in the htable.c file, based on
 * their signatures.
 */
extern void htable_free(htable h);
extern int htable_insert(htable h, char *str);
extern htable htable_new(int capacity, hashing_t method);
extern void htable_print(htable h, void f(int freq, char *str));
extern void htable_print_entire_table(htable h, FILE *stream);
extern int htable_search(htable h, char *str);
extern void htable_print_stats(htable h, FILE *stream, int num_stats);
#endif
