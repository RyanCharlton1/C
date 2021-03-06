#include <stdlib.h>
#include <string.h>


#include "lz77.h"
#include "Maths/ints.h"

//message must be 0 terminated string
char* lz77_encode(const char* str, int windowbits, unsigned int strlen, unsigned int *len){
    unsigned char *msg = NULL;
    unsigned int msglen = 0;

    unsigned int windowsize = powi(2, windowbits);
    unsigned int maxrunlen = powi(2, 16 - windowbits) + 3 - 1;

    unsigned int strptr = 0;
    while(strptr < strlen){
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
                  j < maxrunlen && str[strptr + j])
                j++;

            if(j > runlen){
                runlen = j;
                runindex = i;
            }
        }

        runindex -= start;
        if(runlen > 2){
            int x;
            if(strptr > 1500)
                x = strptr;

            x++;
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
//returns 
char* lz77_decode(const unsigned char* str, int windowbits, unsigned int strlen, unsigned int *len){
    char flagbyte;
    unsigned int windowsize = powi(2, windowbits);
    char *msg = NULL;
    unsigned int msglen = 0;
    

    for(int i = 0; i < strlen; i++){
        if(i % 9 == 0){
            flagbyte = str[i];
        }else{
            int bit = i % 9 - 1;
            if(flagbyte & 0x1 << bit){
                unsigned short int indexlen = 0;
                indexlen += str[i] << 8;
                i++;

                if(i % 9 == 0){
                    flagbyte = str[i];
                    i++;
                }

                indexlen += str[i];
                int runindex = indexlen >> (16 - windowbits);
                int runlen = indexlen % powi(2, 16 - windowbits) + 3;
                
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

    *len = msglen;
    return msg;
}