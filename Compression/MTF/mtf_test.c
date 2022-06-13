#include <stdio.h>
#include <stdlib.h>

#include "Compression/MTF/mtf.h"

int main(){
    FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/chapter.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    unsigned int msglen = ftell(msgfp);
    rewind(msgfp);
    char *msg = malloc(msglen);
    fread(msg, msglen, 1, msgfp);
    fclose(msgfp);

    unsigned char *encoded = mtf_encode(msg, msglen);
    msgfp = fopen("encoded.txt", "w");
    fwrite(encoded, msglen, 1, msgfp);
    fclose(msgfp);

    unsigned char *decoded = mtf_decode(encoded, msglen);
    msgfp = fopen("decoded.txt", "w");
    fwrite(decoded, msglen, 1, msgfp);
    fclose(msgfp);

    free(msg);
    free(encoded);
    free(decoded);
    return 0;
}