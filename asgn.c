#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "tree.h"
#include "htable.h"
#include "mylib.h"

#define TRUE 1
#define FALSE 0

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

static void print_help(){
    
    /* Trying to do formatting to these standards  https://stackoverflow.com/questions/9725675/is-there-a-standard-format-for-command-line-shell-help-text */
    printf("This application reads words from an input file, stores them in a data structure,then prints out the words along with the frequency they appear in the input.\nTo use this application:\n");
    printf("asgn <input_file> [options]\n\t options:\n");
    printf("\t-T\t\t Use a tree data structure (structure defaults to hash table).\n\t-c <filename>\t Check the spelling of words in filename, using the words read from stdin as the dictionary.\n");
    printf("\t-d\t\t Use double hashing (hashing is linear by default).\n");
    printf("\t-e\t\t Display the entire contents of a hash table to stderr.\n");
    printf("\t-o\t\t Outputs a representation of the tree in \"dot\" form to the file 'tree-view.dot'.\n");
    printf("\t-p\t\t Prints stats information instead of words and frequencies.\n");
    printf("\t-r\t\t Makes a tree structure a RBT (trees are BSTs by default).\n");
    printf("\t-s <snapshots>\t Display up to the given number of stats snapshots when given -p as an argument. Snapshots with 0 entires are not shown.\n");
    printf("\t-t <tablesize>\t Use the first prime >= tablesize as the intial hashtable size.\n\t-h\t\t Prints a help message describing how to use the program.\n");
    
           
    exit(EXIT_SUCCESS);
}

static int isPrime(int n){
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

static int find_next_prime(int a){
    int i=a;
    while(1) {
        if(isPrime(i))
            break;
        i++;
    }
    return i;
}


int main(int argc, char **argv){

    const char *optstring = "Tc:deoprs:t:h";
    char option;
    char word[256];
    char *filename;

    tree t;
    htable h;
    int tablesize, snapshots, found;
    FILE *infile;
    FILE *outfile;

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

    while (getword(word, sizeof word, stdin) != EOF){

        if (flag_T == TRUE){
            t = tree_insert(t, word);
        } else{
            htable_insert(h, word);
        }
    }

    if (flag_e == TRUE){
        htable_print_entire_table(h, stderr);
    }

    if (flag_c == TRUE){

        infile = fopen(filename, "r");

       
        if (infile == NULL) { 
            fprintf(stderr, "Error: no file specified");
            return EXIT_FAILURE;
        }
        
        while(getword(word, sizeof word, infile) != EOF){
            found = htable_search(h, word);
          
            if (found == 0){
                fprintf(stdout, "%s\n", word);
            }
        }
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
    }

    if (flag_T == TRUE){
        tree_free(t);
    } else{
        htable_free(h);
    }
    
            
    return EXIT_SUCCESS;     
}
