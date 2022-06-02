#include <stdlib.h>
#include <stdio.h>

#include "Compression/RLE/rle.h"

#define ENCODEDLEN encodedptr + 1

char* rle_encode(const char* str, unsigned long long strlen, unsigned long long *len){
    unsigned char *encoded = NULL;
    unsigned long long encodedptr = 0;
    unsigned long long strptr = 0;
    int x;
    while(strptr < strlen){
        if(encodedptr % 9 == 0){
            encoded = realloc(encoded, ENCODEDLEN);
            encoded[encodedptr] = '\0';
            encodedptr++;
        }

        if(strptr > 470000)
            x = 1 + 1;

        int runlen = 1;
        char c = str[strptr]; 
        while(strptr + runlen < strlen && runlen < 256 + 3 && str[strptr + runlen] == c)
            runlen++;
        
        if(runlen > 2){
            encoded = realloc(encoded, ENCODEDLEN);
            encoded[encodedptr] = runlen - 3;
            encoded[encodedptr / 9 * 9] += 0x01 << encodedptr % 9 - 1;
            encodedptr++;

            if(encodedptr % 9 == 0){
                encoded = realloc(encoded, ENCODEDLEN);
                encoded[encodedptr] = '\0';
                encodedptr++;
            }

            strptr += runlen - 1;
        }
        
        encoded = realloc(encoded, ENCODEDLEN);
        encoded[encodedptr] = c;
        encodedptr++;
        strptr++;
    }

    *len = encodedptr;
    return encoded;
}

char* rle_decode(const unsigned char* str, unsigned long long strlen, unsigned long long *len){
    unsigned long long strptr = 0;
    unsigned char *decoded = NULL;
    unsigned long long decodedlen = 0;

    while(strptr < strlen){
        if(strptr % 9 == 0)
            strptr++;
        
        if(str[strptr / 9 * 9] & 0x1 << strptr % 9 - 1){
            int runlen = str[strptr] + 3;
            strptr++;

            if(strptr % 9 == 0)
                strptr++;

            char c = str[strptr];
            for(int i = 0; i < runlen; i++){
                decodedlen++;
                decoded = realloc(decoded, decodedlen);
                decoded[decodedlen - 1] = c;
            }

            strptr++;
        }else{
            decodedlen++;
            decoded = realloc(decoded, decodedlen);
            decoded[decodedlen - 1] = str[strptr];
            strptr++;
        }
    }

    *len = decodedlen;
    return decoded;
}
