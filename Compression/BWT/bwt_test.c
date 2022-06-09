#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Compression/BWT/bwt.h"
#define KiB 1024

int main(){
    char test[13] = "IPSSM\255PISSII";
    bwt_decode(test, 13);
    char * encoeded = bwt_encode(test, 12);
    char* msg;
    
    /*FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/chapter.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    //unsigned int msglen = ftell(msgfp);
    unsigned int msglen = 8 * KiB;
    rewind(msgfp);
    msg = malloc(msglen + 1);
    fread(msg, msglen, 1, msgfp);
    msg[msglen] = 0;
    fclose(msgfp);

    char *bwt = bwt_encode(msg, msglen + 1);
    free(msg);

    FILE *bwtfp = fopen("encoded.txt", "w");
    fwrite(bwt, msglen + 1, 1, bwtfp);
    fclose(bwtfp);

    bwt_decode(bwt, msglen + 1);
    bwtfp = fopen("decoded.txt", "w");
    fwrite(bwt, msglen, 1, bwtfp);
    fclose(bwtfp);

    free(bwt);
    */
    return 0;
}