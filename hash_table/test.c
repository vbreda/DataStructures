#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(void){

    htable table = htable_new(10);
    
    htable_free(table);

    return EXIT_SUCCESS;
}
