#include "Compression/BWT/bwt.h"

//returns 1 if x is greater than y 0 otherwise
int compare_cycles(const unsigned char* str, unsigned int strlen, unsigned int x, unsigned int y){
    for(unsigned int i = 0; i < strlen; i++){
        if(str[x + i % strlen] > str[y + i % strlen])
            return 1;
        if(str[x + i % strlen] < str[y + i % strlen])
            return 0;
    }
}

//takes a string ended by 255
char *bwt_encode(const unsigned char *str, unsigned int strlen){
    char *encoded = malloc(strlen + 1);
    encoded[strlen] = (char)255; //end of stream char, rle adjusted to not create one

    unsigned int place;
    for(unsigned int i = 0; i < strlen; i++){
        place = 0;
        for(unsigned int j = 0; j < strlen; j++){
            if(i != j)
                place += compare_cycles(str, strlen, i, j);
        }
         encoded[place] = str[i];
    }

    return encoded;
}