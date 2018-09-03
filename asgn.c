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
    printf("This application reads words from an input file, stores them in a data structure, then prints out the words along with the frequency they appear in the input.\n
To use this application:\n
asgn <input_file> [options]\n
\t options:\n
\t\t-T\t\t Use a tree data structure (structure defaults to hash table).\n
\t\t-c <filename>\t Check the spelling of words in filename, using the words read from stdin as the dictionary.\n
\t\t-d\t\t Use double hashing (hashing is linear by default).\n
\t\t-e\t\t Display the entire contents of a hash table to stderr.\n
\t\t-o\t\t Outputs a representation of the tree in \"dot\" form to the file 'tree-view.dot'.\n
\t\t-p\t\t Prints stats information instead of words and frequencies.\n
\t\t-r\t\t Makes a tree structure a RBT (trees are BSTs by default).\n
\t\t-s <snapshots>\t Display up to the given number of stats snapshots when given -p as an argument. Snapshots with 0 entires are not shown.\n
\t\t-t <tablesize>\t Use the first prime >= tablesize as the intial hashtable size.\n
\t\t-h\t\t Prints a help message describing how to use the program.\n")
           

}


int main(void){

    const char *optstring = "ab:c";
    char option;

    int flag_T = FALSE;
    int flag_c = FALSE;
    int flag_e = FALSE;
    int flag_o = FALSE;
    int flag_p = FALSE;
    int flag_r = FALSE;
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
                flag_c = TRUE;
                break;
            case 'd':
                /* Use double hashing as the collison resolution strategy
                 (linear is the default)*/
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
                /* Make the tree and rbt instead of the default bst. */
                flag_r = TRUE;
                break;
            case 's':
                /* Display up to the given number of stats snapchots when
                   given -p as an argument. If the table is not full then
                   fewer snapshots will be displayed. Snapshots with 0 entires
                   are not shown. */
                break;
            case 't':
                /* Use the first prime >= tablesize as the size of your hash
                   table. You can assume the tablesize will be a number
                   greater then 0. */
                flag_t = TRUE;
                break;
            case 'h':
                /* Print a help message describing how to use the program. */
                print_help();
                break;
                
            default:
                /* if an unknown option is given */
                break;
        }
    }
