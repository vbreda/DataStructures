/**
 * File: htable.h
 * @author Vivian Breda, Josh King, Abinaya Saravanapavan.
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdio.h>

/**
 * Prototypes
 * Purpose: specifies functions to be implemented in the mylib.c file, based on
 * their signatures.
 */
extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *,int,FILE *);

#endif
