#include <stdio.h>

#include "suffix_array.h"

#define STRLEN 7

int main(){
    const char *str = "banana\255";
    
    SuffixArray *sa = init_suffixarr(str, STRLEN);
    print_suffixarr_char(sa);

    bwt_encode_suffixarr(sa);
    print_suffixarr_int(sa);
    print_suffixarr_char(sa);

    for(int i = 0; i < sa->size; i++)
        printf("%c ", str[sa->indices[i] == 0 ? STRLEN - 1 : sa->indices[i] - 1 % STRLEN]);
    putchar('\n');

    free_suffixarr(sa);
    return 0;
}