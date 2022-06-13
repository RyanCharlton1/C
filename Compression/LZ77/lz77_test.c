#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Compression/LZ77/lz77.h"

int main(){
    //hello world
    char* msg;
    FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/MTF/encoded.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    int msglen = ftell(msgfp);
    rewind(msgfp);
    msg = malloc(msglen + 1);
    fread(msg, msglen, 1, msgfp);
    msg[msglen] = 0;
    fclose(msgfp);

    //const char *msg = ", Privet Drive, were proud to say that they were perfectly normal, thank you very much. They were the last people youd expect to be involved in anything strange or mysterious, because they just didnt hold with such nonsense.";
    int encodeln;
    unsigned char *encoded = lz77_encode(msg, 12, msglen, &encodeln);
    
    FILE *encodefp = fopen("encoded.txt", "w");
    fwrite(encoded, encodeln, 1, encodefp);
    fclose(encodefp);
    unsigned int *decodelen;
    char *decoded = lz77_decode(encoded, 12, encodeln, &decodelen);


    FILE *decodefp = fopen("decoded.txt", "w");
    fwrite(decoded, msglen, 1, decodefp);
    fclose(decodefp);

    free(msg);
    free(encoded);
    free(decoded);
    return 0;
}