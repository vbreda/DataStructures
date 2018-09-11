/**
 * File: htable.c
 * @author: Vivian Breda, Josh King, Abinaya Saravanapavan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "tree.h"
#include "htable.h"
#include "mylib.h"

#define TRUE 1
#define FALSE 0

/**
 * Function: print_info
 * Purpose: prints the frequency of a word and the word itself.
 *
 * @param freq is the frequency of the word. 
 * @param word is a pointer to the word in question. 
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 * Function: print_help
 * Purpose: prints a helpful board listing the choices a user can take. 
 */
static void print_help(){
    
    /* Trying to do formatting to these standards  https://stackoverflow.com/questions/9725675/is-there-a-standard-format-for-command-line-shell-help-text */
  printf("Usage: ./asgn [OPTIONS]... <STDIN>\n\n");
  
  printf("Perform tasks using a hash table or binary tree.  By default, words\n");
  printf("read from stdin are added to the data structure before printing\n");
  printf("them, along with their frequencies, to stdout.\n\n");
  
  printf("-T           Use a tree data structure (deafult is hash table)\n");
  printf("-c FILENAME  Check the spelling of words in FILENAME using the words\n");
  printf("             read from stdin as dictionary. Print unknown words to\n");
  printf("             stdout, timing info etc to stderr (ignore -o & -p)\n");
  printf("-d           Use double hashing (linear probing is the default)\n");
  printf("-e           Display the entire contents of hash table to stderr\n");
  printf("-o           Output the tree in DOT form to  file 'tree-view.dot'\n");
  printf("-p           Print hash table stats instead of frequencies & words\n");
  printf("-r           Makes the tree an RBT (the default is a BST)\n");
  printf("-s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n");
  printf("-t TABLESIZE Use the first prime >= TABLESIZE as htable size.\n\n");
  printf("-h           Display this message\n\n");
    
           
    exit(EXIT_SUCCESS);
}

/**
 * Function: is_prime
 * Purpose: determines whether an integer is a prime number. 
 *
 * @param n is the integer whos prime status is to be determined. 
 * @return 1 if prime, 0 if not prime. 
 */
static int is_prime(int n){
    int i,j=0;
    for(i=1; i<=n; i++){
        if(n%i == 0)
            j++;
    }
    if(j == 2){
        return 1;
    }
    else if(j > 2){
        return 0;
    }
    return 0;
}

/**
 * Function: find_next_prime
 * Purpose: finds the next prime number. 
 *
 * @param a is the integer from which the search starts. 
 * @return i the integer which is the next prime after a. 
 */
static int find_next_prime(int a){
    int i=a;
    while(1) {
        if(is_prime(i))
            break;
        i++;
    }
    return i;
}

/**
 * Main method
 */ 
int main(int argc, char **argv){

    const char *optstring = "Tc:deoprs:t:h";
    char option;
    char word[256];
    char *filename;

    tree t;
    htable h;
    int tablesize, snapshots, found, unknown;
    FILE *infile;
    FILE *outfile;
    clock_t start, end;
    double fill_time = 0.0, search_time = 0.0;

    int flag_T = FALSE;
    int flag_c = FALSE;
    int flag_d = FALSE;
    int flag_e = FALSE;
    int flag_o = FALSE;
    int flag_p = FALSE;
    int flag_r = FALSE;
    int flag_s = FALSE;
    int flag_t = FALSE;
    

    
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'T':
                /* Use a tree data structure (default is hashtable) */
                flag_T = TRUE;
                break;
            case 'c':
                /* Check the spelling of words in filename using words read
                   from stdin as the dictionary. Print all unknown words to
                   stdout. Print timing information and unknown word count to
                   stderr. When this option is given the -p or -o options
                   should be ignored. */
                filename = optarg;
                flag_c = TRUE;
                break;
            case 'd':
                /* Use double hashing as the collison resolution strategy
                 (linear is the default)*/
                flag_d = TRUE;
                break;
            case 'e':
                /* Display entire contents of hash table on stderr using
                   the format string "%5d %5d %5d   %s\n" to display the
                   index, frequency, stats and the key if it exists. */
                flag_e = TRUE;
                break;
            case 'o':
                /* Output a representation of the tree in 'dot' form to the
                   file 'tree-view.dot' using the functions given in
                   out-dot.txt */
                flag_o = TRUE;
                break;
            case 'p':
                /* Print stats info using the functions provided in
                   print-stats.txt instead of printing the frequencies
                   and words. */
                flag_p = TRUE;
                break;
            case 'r':
                /* Make the tree an rbt instead of the default bst. */
                flag_r = TRUE;
                break;
            case 's':
                /* Display up to the given number of stats snapchots when
                   given -p as an argument. If the table is not full then
                   fewer snapshots will be displayed. Snapshots with 0 entires
                   are not shown. */
                flag_s = TRUE;
                snapshots = atoi(optarg);
                break;
            case 't':
                /* Use the first prime >= tablesize as the size of your hash
                   table. You can assume the tablesize will be a number
                   greater then 0. */
                flag_t = TRUE;
                tablesize = atoi(optarg);
                break;
            case 'h':
                /* Print a help message describing how to use the program. */
                print_help();
                break;
            default:
                /* if an unknown option is given */
                print_help();
              break;
        }
    }

    if (flag_T == TRUE){
        if (flag_r == TRUE){
            t = tree_new(RBT);
        } else{
            t = tree_new(BST);
        }
    } else{
        if (flag_t == TRUE){
            tablesize = find_next_prime(tablesize);
        } else{
            tablesize = 113;
        }
        if (flag_d == TRUE){
            h = htable_new(tablesize, DOUBLE_H);
        } else{
            h = htable_new(tablesize, LINEAR_P);
        }
    }
    
    start = clock();
    
    while (getword(word, sizeof word, stdin) != EOF){

        if (flag_T == TRUE){
            t = tree_insert(t, word);
        } else{
            htable_insert(h, word);
        }
    }
    
    end = clock();
    fill_time = (end - start) / (double) CLOCKS_PER_SEC;
    
    if (flag_e == TRUE && flag_T == FALSE){
        htable_print_entire_table(h, stderr);
    }

    if (flag_c == TRUE){
        unknown = 0;
        infile = fopen(filename, "r");
       
        if (infile == NULL) { 
            fprintf(stderr, "Error: no file specified");
            return EXIT_FAILURE;
        }
        
        start = clock();
        
        while(getword(word, sizeof word, infile) != EOF){

            if(flag_T == TRUE){
                found = tree_search(t, word);
            }
            else{
                found = htable_search(h, word);
            }
            
            if (found == 0){
                fprintf(stdout, "%s\n", word);
                unknown++;
            }
        }
        
        end = clock();
        search_time = (end - start) / (double) CLOCKS_PER_SEC;
        fclose(infile);
        
        fprintf(stderr, "Fill time     : %f\n", fill_time);
        fprintf(stderr, "Search time   : %f\n", search_time);
        fprintf(stderr, "Unknown words = %d\n", unknown);
    }
    
    else if (flag_p == TRUE && flag_T == FALSE && flag_c == FALSE){
        
        if (flag_s == TRUE){
            htable_print_stats(h, stdout, snapshots);
        } else{
            htable_print_stats(h, stdout, 10);
        }
    }
    else {
        if (flag_T == TRUE){
            tree_preorder(t, print_info);
        }
        else{
            htable_print(h, print_info);
        }
    }

    if (flag_o == TRUE && flag_T == TRUE && flag_c == FALSE){

        printf("Creating dot file 'tree-view.dot'\n");
        outfile = fopen("tree-view.dot", "w");
        tree_output_dot(t, outfile);
        fclose(outfile);
    }

    if (flag_T == TRUE){
        tree_free(t);
    } else{
        htable_free(h);
    }
    
            
    return EXIT_SUCCESS;     
}
