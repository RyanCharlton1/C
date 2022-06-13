#include <stdlib.h>
#include <stdio.h>

#include "Compression/MTF/mtf.h"

unsigned char *mtf_encode(unsigned char *str, unsigned int strlen){
    unsigned char *alphabet = malloc(256);
    for(int i = 0; i < 256; i++)
        alphabet[i] = i;

    unsigned char *encoded = malloc(strlen);
    for(int i = 0; i < strlen; i++){
        int n = 0;
        while(alphabet[n] != str[i])
            n++;
        
        unsigned char tmp = alphabet[n];
        encoded[i] = n;

        for(int j = n; j != 0; j--)
            alphabet[j] = alphabet[j - 1];
        alphabet[0] = tmp;
    }

    free(alphabet);
    return encoded;
}

unsigned char *mtf_decode(unsigned char *str, unsigned int strlen){
    char *alphabet = malloc(256);
    for(int i = 0; i < 256; i++)
        alphabet[i] = i;

    unsigned char *decoded = malloc(strlen);
    for(int i = 0; i < strlen; i++){
        unsigned char tmp = alphabet[str[i]];
        decoded[i] = tmp;

        for(int j = str[i]; j != 0; j--)
            alphabet[j] = alphabet[j - 1];
        alphabet[0] = tmp;
    }

    free(alphabet);
    return decoded;
}
