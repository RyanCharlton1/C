#include <stdio.h>
#include <stdlib.h>

#include "Compression/LZMA/lzma.h"

int main(){

    char* msg;
    FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/bible.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    unsigned long msglen = ftell(msgfp);
    rewind(msgfp);
    msg = malloc(msglen);
    fread(msg, msglen, 1, msgfp);
    fclose(msgfp);

    int encodedlen;
    char *encoded = lzma_encode(msg, msglen, &encodedlen, 12);

    msgfp = fopen("encoded.txt", "w");
    fwrite(encoded, encodedlen, 1, msgfp);
    fclose(msgfp);

    char *decoded = lzma_decode(encoded, encodedlen);

    msgfp = fopen("decoded.txt", "w");
    fwrite(decoded, msglen, 1, msgfp);
    fclose(msgfp);

    free(encoded);
    free(decoded);
    free(msg);
    return 0;
}