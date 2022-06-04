#include <stdio.h>

#include "suffix_array.h"

#define STRLEN 8

int main(){
    const char *str = "banana\255";

    SuffixArray *sa = init_suffixarr(str, STRLEN);
    for(int i = 0; i < STRLEN; i++)
        printf("%c ", str[sa->indices[i]]);
    putchar('\n');

    bwt_encode_suffixarr(sa);
    for(int i = 0; i < STRLEN; i++)
        printf("%c ", str[sa->indices[i]]);
    putchar('\n');

    free_suffixarr(sa);
    return 0;
}