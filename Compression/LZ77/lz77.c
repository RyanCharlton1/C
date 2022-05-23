#include <stdlib.h>
#include <string.h>


#include "lz77.h"

int pow(int x, int y){
    if(y == 0)
        return 1;

    return x * pow(x, y - 1);
}

//message must be 0 terminated string
char* lz77_encode(const char* str, int windowbits, int *len){
    unsigned char *msg = NULL;
    unsigned int msglen = 0;

    unsigned int windowsize = pow(2, windowbits);
    unsigned int maxrunlen = pow(2, 16 - windowbits) + 3;

    unsigned int strsize = strlen(str);
    unsigned int strptr = 0;
    while(strptr < strsize){
        //flag byte
        if(msglen % 9 == 0){
            msglen++;
            msg = realloc(msg, msglen * sizeof(char));
            msg[msglen - 1] = '\0'; //init to 0000 0000(raw char)
        }

        int start = strptr - windowsize;
        if(start < 0)
            start = 0;

        //scan window longest run
        int runlen = 0;
        int runindex = 0;
        for(int i = start; i < strptr; i++){
            int rl = 0;
            int j = 0;
            while(str[i + j] == str[strptr + j] && \
                  j <= maxrunlen && str[i + j] && str[strptr + j])
                j++;

            if(j > runlen){
                runlen = j;
                runindex = i;
            }
        }

        if(runlen > 2){
            int c = 0;
            c += runindex << (16 - windowbits);
            c += runlen - 3;


            //flag byte
            if(msglen % 9 == 0){
                msglen++;
                msg = realloc(msg, msglen * sizeof(char));
                msg[msglen - 1] = '\0'; //init to 0000 0000(raw char)
            }

            msg[msglen / 9 * 9] += 0x1 << (msglen % 9 - 1);
            msglen++;
            msg = realloc(msg, msglen * sizeof(char));
            msg[msglen - 1] = c >> 8; 
        

            //flag byte
            if(msglen % 9 == 0){
                msglen++;
                msg = realloc(msg, msglen * sizeof(char));
                msg[msglen - 1] = '\0'; //init to 0000 0000(raw char)
            }

            msg[msglen / 9 * 9] += 0x1 << (msglen % 9 - 1);
            msglen++;
            msg = realloc(msg, msglen * sizeof(char));
            char ch = c & 0xFF;
            msg[msglen - 1] = ch; 

            strptr += runlen;
        }
        else{
            //flag byte
            if(msglen % 9 == 0){
                msglen++;
                msg = realloc(msg, msglen * sizeof(char));
                msg[msglen - 1] = '\0'; //init to 0000 0000(raw char)
            }

            msglen++;
            msg = realloc(msg, msglen * sizeof(char));
            msg[msglen - 1] = str[strptr];
        
            strptr++;
        }
    }
    *len = msglen;
    return msg;
}

char* lz77_decode(const char* str, int windowbits, int len){
    char flagbyte;
    unsigned int windowsize = pow(2, windowbits);
    char *msg = NULL;
    unsigned int msglen = 0;
    int indexlen = 0;

    for(int i = 0; i < len; i++){
        if(i % 9 == 0){
            flagbyte = str[i];
        }else{
            int bit = i % 9 - 1;
            if(flagbyte & 0x1 << bit){
                indexlen += str[i] << 8;
                i++;

                if(i % 9 == 0){
                    flagbyte = str[i];
                    i++;
                }

                indexlen += str[i];
                int runindex = indexlen >> (16 - windowbits);
                int runlen = indexlen % pow(2, 16 - windowbits) + 3;
                
                int msgptr = msglen - windowsize;
                if(msgptr < 0)
                    msgptr = 0;

                for(int j = 0; j < runlen; j++){
                    msglen++;
                    msg = realloc(msg, msglen * sizeof(char));
                    msg[msglen - 1] = msg[msgptr + runindex + j];
                }
            }else{
                msglen++;
                msg = realloc(msg, msglen * sizeof(char));
                msg[msglen - 1] = str[i];
            }
        }
    }

    msglen++;
    msg = realloc(msg, msglen * sizeof(char));
    msg[msglen - 1] = '\0';

    return msg;
}