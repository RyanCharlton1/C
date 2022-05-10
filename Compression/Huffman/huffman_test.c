#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

int main(){
    char* msg;
    FILE *msgfp = fopen("message.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    int msglen = ftell(msgfp);
    rewind(msgfp);
    msg = malloc(msglen);
    fread(msg, msglen, 1, msgfp);
    fclose(msgfp);

    int encodeln, decodeln;
    char *encoded = huffman_encode(msg, &encodeln);
    
    FILE *encodefp = fopen("encoded.txt", "w");
    fwrite(encoded, encodeln, 1, encodefp);
    fclose(encodefp);

    char *decoded = huffman_decode(encoded, &decodeln, encodeln);

    FILE *decodefp = fopen("decoded.txt", "w");
    fwrite(decoded, decodeln, 1, decodefp);
    fclose(decodefp);

    free(encoded);
    free(decoded);
    return 0;
}
