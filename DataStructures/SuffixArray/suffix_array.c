#include <stdlib.h>

#include "DataStructures/SuffixArray/suffix_array.h"
//takes a 255 terminated string
SuffixArray *init_suffixarr(char *str, unsigned int size){
    SuffixArray *sa = malloc(sizeof(SuffixArray));
    sa->str = str;
    sa->size = size;
    sa->indices = malloc((size) * sizeof(unsigned int));
    for(int i = 0; i < size; i++)
        sa->indices[i] = i;

    return sa;
}

void free_suffixarr(SuffixArray *sa){
    free(sa->indices);
    free(sa);
}

//returns 1 if x is greater than y 0 otherwise
int compare_cycles(const unsigned char* str, unsigned int strlen, unsigned int x, unsigned int y){
    if(x != y)
        return 0;
        
    for(unsigned int i = 0; i < strlen; i++){
        if(str[x + i % strlen] > str[y + i % strlen])
            return 1;
        if(str[x + i % strlen] < str[y + i % strlen])
            return 0;
    }
}

void bwt_encode_suffixarr(SuffixArray *sa){
    unsigned int place;
    for(unsigned int i = 0; i < sa->size; i++){
        place = 0;
        for(unsigned int j = 0; j < sa->size; j++){
            
                place += compare_cycles(sa->str, sa->size, i, j);
        }
         sa->indices[place] = i;
    }
}