#include <stdlib.h>
#include <string.h>

#include "Compression/BWT/bwt.h"

//returns 1 if x is greater than y 0 otherwise
int compare_cycles(const unsigned char* str, unsigned int strlen, unsigned int x, unsigned int y){
    for(unsigned int i = 0; i < strlen + 1; i++){
        if(str[(x + i) % strlen] > str[(y + i) % strlen])
            return 1;
        if(str[(x + i) % strlen] < str[(y + i) % strlen])
            return 0;
    }
}
//must give string ending in \255
//find each cycles order then puts cycles final char in encoded
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
        encoded[place] = (i == 0 ? '\255' : str[i - 1]);
    }

    return encoded;
}

int compare_chars(const void *x, const void *y){
    return *((unsigned char*)x) > *((unsigned char*)y);
}
//algorithm described here:
//https://www.sciencedirect.com/science/article/pii/S1570866715000052
void bwt_decode(unsigned char* str, unsigned int strlen){
    unsigned int f, p, count;
    long int q;
    unsigned char c;

    p = 0;
    while(str[p] != (unsigned char)'\255')
        p++;
    p++;

    char *charset = malloc(strlen);
    memcpy(charset, str, strlen);
    qsort(charset, strlen, 1, compare_chars);   
    while(strlen > 2){
        
        c = charset[p - 1];
        for(int i = p - 1; i < strlen - 1; i++)
            charset[i] = charset[i + 1];
        realloc(charset, strlen - 1);

        count = 0;
        for(int i = 0; i < strlen; i++)
            if(str[i] < c) 
                count++;

        f = p - count;
        q = -1;
        while(f > 0){
            q++;
            if(str[q] == c)
                f--;
        }

        str[q] = '\255';
        for(int i = p - 1; i > 0; i--)
            str[i] = str[i - 1];
        
        str[0] = c;
        str++; strlen--;
        
        if(p - 1 > q)
            p = q + 1;
        else
            p = q;
    } 

    free(charset);
}