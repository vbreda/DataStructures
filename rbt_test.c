#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"
#include "mylib.h"

void print_key(char *s, rbt_colour c)
{
    if (c == BLACK){
        printf("black:\t%s\n", s);
    }else{
        printf("red:\t%s\n", s);
    }
}

int main(void){
    rbt b;
    char word[256];
    char op;

    b = rbt_new();

    while (2 == scanf(" %c %255s", &op, word)) {
        if ('+' == op) {
            b = rbt_insert(b, word);
        } else if ('?' == op) {
            /*printf("%d\n", rbt_search(b, word));*/
        }
    }

    rbt_preorder(b, print_key);
    
   
    b = rbt_free(b);

    return EXIT_SUCCESS;
}
