#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Compression/LZ77/lz77.h"

int main(){
    /*
    char* msg;
    FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/chapter.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    int msglen = ftell(msgfp);
    rewind(msgfp);
    msg = malloc(msglen);
    fread(msg, msglen, 1, msgfp);
    fclose(msgfp);
*/
    const char *msg = ", Privet Drive, were proud to say that they were perfectly normal, thank you very much. They were the last people youd expect to be involved in anything strange or mysterious, because they just didnt hold with such nonsense.";
    int encodeln;
    char *encoded = lz77_encode(msg, 8, &encodeln);
    
    FILE *encodefp = fopen("encoded.txt", "w");
    fwrite(encoded, encodeln, 1, encodefp);
    fclose(encodefp);

    char *decoded = lz77_decode(encoded, 8, encodeln);

    FILE *decodefp = fopen("decoded.txt", "w");
    fwrite(decoded, strlen(decoded), 1, decodefp);
    fclose(decodefp);

    free(encoded);
    free(decoded);
    return 0;
    return 0;
}