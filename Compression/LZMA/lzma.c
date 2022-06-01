#include <stdio.h>
#include <stdlib.h>

#include "DataStructures/BitArray/bit_array.h"
#include "Maths/ints.h"
#include "Compression/LZMA/lzma.h"

void write_length_bitarr(BitArray *ba, int len){
    if(2 <= len && len <= 9){
        write_bitarr(ba, 0);
        int adjusted = len - 2;
        for(int i = 2; i >= 0; i--)
            write_bitarr(ba, adjusted & 0x1 << i);
    }
    if(10 <= len && len <= 17){
        write_bitarr(ba, 1);
        write_bitarr(ba, 0);
        int adjusted = len - 10;
        for(int i = 2; i >= 0; i--)
            write_bitarr(ba, adjusted & 0x1 << i);
    }
    if(18 <= len && len <= 273){
        write_bitarr(ba, 1);
        write_bitarr(ba, 1);
        int adjusted = len - 18;
        for(int i = 7; i >= 0; i--)
            write_bitarr(ba, adjusted & 0x1 << i);
    }
}

void write_distance_bitarr(BitArray *ba, unsigned long dist){
    int bits = log2i(dist) + 1;
    for(int i = 5; i >= 0; i--)
        write_bitarr(ba, bits & 0x1 << i);
    if(dist > 1)
        for(int i = bits - 1; i >= 0; i--)
            write_bitarr(ba, dist & 0x01 << i);
}

int read_length_bitarr(BitArray *ba){
    int len;
    if(!read_bitarr(ba)){
        //0
        len = 2;
        for(int i = 2; i >= 0; i--)
            len += read_bitarr(ba) << i;
    }else{
        //1
        if(!read_bitarr(ba)){
            //10
            len = 10;
            for(int i = 2; i >= 0; i--)
                len += read_bitarr(ba) << i;
        }else{
            //11
            len = 18;
            for(int i = 7; i >= 0; i--)
                len += read_bitarr(ba) << i;
        }
    }

    return len;
}

unsigned long read_distance_bitarr(BitArray *ba){
    int bits = 0;
    for(int i = 5; i >= 0; i--)
        bits += read_bitarr(ba) << i;

    unsigned long dist = 0;
    for(int i = bits - 1; i >= 0; i--)
        dist += read_bitarr(ba) << i;

    return dist;
}

char* lzma_encode(const char *str, unsigned long strlen, unsigned long *msglen, int windowbits){
    BitArray *ba = init_bitarr();
    unsigned long *longrep = malloc(4 * sizeof(unsigned long));
    for(int i = 0; i < 4; i++)
        longrep[i] = -1;                //-1 unused

    unsigned long windowsize = powi(2, windowbits);
    unsigned long strptr = 0;
    while(strptr < strlen){

        unsigned long start = strptr - windowsize;
        if(windowsize > strptr)
            start = 0;
        
        int runlen = 0;
        unsigned long runindex = 0;
        for(long unsigned i = start; i < strptr; i++){
            int j = 0;
            while(str[i + j] == str[strptr + j] && \
                  j < windowsize && strptr + j < strlen)
                j++;

            if(j > runlen){
                runlen = j;
                runindex = i;
            }
        }

        unsigned long dist = strptr - runindex - 1;
        int chunk;
        if(runlen <= 1){
            chunk = LITERAL;
            if(-1 < longrep[0])
                if(str[strptr - longrep[0]] == str[strptr])
                    chunk = SHORTREP;
            
        }else if(longrep[0] == dist){
            chunk = LONGREP0;
        }else if(longrep[1] == dist){
            chunk = LONGREP1;
        }else if(longrep[2] == dist){
            chunk = LONGREP2;
        }else if(longrep[3] == dist){
            chunk = LONGREP3;
        }else{
            chunk = MATCH;
        }

        switch (chunk){
        case LITERAL:
            write_bitarr(ba, 0);
            write_char_bitarr(ba, str[strptr]);
            strptr++;
            break;

        case MATCH:
            write_bitarr(ba, 1);
            write_bitarr(ba, 0);
            write_length_bitarr(ba, runlen);
            write_distance_bitarr(ba, dist);

            for(int i = 3; i > 0; i--)
                longrep[i] = longrep[i - 1];
            longrep[0] = dist;
            
            strptr += runlen;
            break;

        case SHORTREP:
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 0);
            write_bitarr(ba, 0);
            strptr++;
            break;

        case LONGREP0:
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 0);
            write_bitarr(ba, 1);
            write_length_bitarr(ba, runlen);
            strptr += runlen;
            break;

        case LONGREP1:
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 0);
            write_length_bitarr(ba, runlen);
            strptr += runlen;
            break;

        case LONGREP2:
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 0);
            write_length_bitarr(ba, runlen);
            strptr += runlen;
            break;

        case LONGREP3:
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_bitarr(ba, 1);
            write_length_bitarr(ba, runlen);
            strptr += runlen;
            break;
        }
    }

    write_bitarr(ba, 0);
    write_char_bitarr(ba, EOT);

    *msglen = ba->bytecount;

    free(longrep);
    char *temp = ba->data;
    free(ba);
    return temp;
}

char* lzma_decode(const char *str, unsigned long strlen){
    BitArray *ba = init_bitarr_string(str, strlen);
    unsigned long *longrep = malloc(4 * sizeof(unsigned long));

    unsigned long msglen = 0;
    unsigned char *msg = NULL;

    int end = 0;
    while(!end){

        int chunk = read_bitarr(ba);
        while(chunk != LITERAL && chunk != MATCH && chunk != SHORTREP &&\
              chunk != LONGREP0 && chunk != LONGREP1 && chunk != LONGREP2&&\
              chunk != LONGREP3){
            chunk *= 10;
            chunk += read_bitarr(ba);
        }
       
        int index = 0;
        int len;
        unsigned long runstart;
        switch (chunk)
        {
        case LITERAL:
            unsigned char c = read_char_bitarr(ba);
            if(c == EOT){
                end = 1;
                break;
            }
            msglen++;
            msg = realloc(msg, msglen);
            msg[msglen - 1] = c;
            break;
        
        case MATCH:
            len = read_length_bitarr(ba);
            unsigned long dist = read_distance_bitarr(ba);
            runstart = msglen - dist - 1;

            for(int i = 3; i > 0; i--)
                longrep[i] = longrep[i - 1];
            longrep[0] = dist;

            for(int i = 0; i < len; i++){
                msglen++;
                msg = realloc(msg, msglen);
                msg[msglen - 1] = msg[runstart + i];
            }
            break;

        case SHORTREP:
            runstart = msglen - longrep[0] - 1;
            msglen++;
            msg = realloc(msg, msglen);
            msg[msglen - 1] = msg[runstart];
            break;
        
        case LONGREP3:
            index++;
        case LONGREP2:
            index++;
        case LONGREP1:
            index++;
        case LONGREP0:
            int len = read_length_bitarr(ba);
            runstart = msglen - longrep[index] - 1;
            
            for(int i = 0; i < len; i++){
                msglen++;
                msg = realloc(msg, msglen);
                msg[msglen - 1] = msg[runstart + i];
            }
            break;
        }
    }

    free_bitarr(ba);
    free(longrep);
    return msg;
}