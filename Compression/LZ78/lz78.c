#include <stdlib.h>
#include <string.h>

#include "lz78.h"

int begins_with(char* str, char* exp){
    if(strlen(str) < strlen(exp))
        return 0;
    
    int begins = 1;
    for(int i = 0; i < strlen(exp); i++){
        if(str[i] != exp[i])
            begins = 0;
    }

    return begins;
}

//message must be a 0 terminated string
char* lz78_encode(const char* str, int *len){
    unsigned char entries = 1;

    Dict dictionary = malloc(sizeof(Entry));
    dictionary[0].c = EOT;
    dictionary[0].index = 0;
    dictionary[0].str = malloc(sizeof(char));
    dictionary[0].str[0] = '\0';
    int i = 0;
    while(i < strlen(str)){
        int explen = 0;
        unsigned char index = 0;
        for(int j = 1; j < entries; j++){
            if(begins_with(&str[i], dictionary[j].str)){
                explen = strlen(dictionary[j].str);
                index = j;
            }
        }

        entries++;
        dictionary = realloc(dictionary, entries * sizeof(Entry));
        dictionary[entries - 1].c = str[i + explen];
        dictionary[entries - 1].index = index;
        int len = strlen(dictionary[index].str) + 2;
        dictionary[entries - 1].str = malloc(len);
        strcpy(dictionary[entries - 1].str, dictionary[index].str);
        dictionary[entries - 1].str[len - 1] = '\0';
        dictionary[entries - 1].str[len - 2] = str[i];
        i += explen + 1;
    }

    int encodedlen = 0;
    char *encoded = malloc(0);

    for(int i = 1; i < entries; i++){
        encodedlen += 2;
        encoded = realloc(encoded, encodedlen * sizeof(char));

        encoded[(i - 1) * 2] = dictionary[i].index;
        encoded[(i - 1) * 2 + 1] = dictionary[i].c;

        free(dictionary[i].str);
    }
    
    *len = encodedlen;
    free(dictionary[0].str);
    free(dictionary);
    return encoded;
}

char* lz78_decode(const char* str, int len, int* decodelen){
    Dict dictionary = malloc(sizeof(Entry));
    
    dictionary[0].index = 0;
    dictionary[0].c = EOT;
    dictionary[0].str = malloc(sizeof(char));
    dictionary[0].str[0] = '\0';

    for(int i = 0; i < len; i += 2){
        dictionary = realloc(dictionary, (i / 2 + 2) * sizeof(Entry));
        unsigned char index = str[i];
        char c = str[i + 1];

        dictionary[i / 2 + 1].index = index;
        dictionary[i / 2 + 1].c = c;

        int size = strlen(dictionary[index].str) + 2;
        dictionary[i / 2 + 1].str = malloc(size * sizeof(char));
        strcpy(dictionary[i / 2 + 1].str, dictionary[index].str);
        dictionary[i / 2 + 1].str[size - 1] = '\0';
        dictionary[i / 2 + 1].str[size - 2] = c;
    }

    char *msg = malloc(sizeof(char));
    msg[0] = '\0';
    int msgsize = 1;

    for(int i = 1; i < len / 2 + 1; i++){
        int size = strlen(dictionary[i].str);
        msgsize += size;
        msg = realloc(msg, msgsize * sizeof(char));

        for(int j = 0; j < size; j++){
            msg[msgsize - 1 - size + j] = dictionary[i].str[j];
        }
        msg[msgsize - 2] = dictionary[i].c;
        free(dictionary[i].str);
    }

    msg[msgsize - 1] = '\0';
    *decodelen = msgsize;
    free(dictionary[0].str);
    free(dictionary);
    return msg;
}