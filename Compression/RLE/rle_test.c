#include <stdio.h>
#include <stdlib.h>

#include "Compression/RLE/rle.h"

#define KB 1024

int main(){
    FILE *fp = fopen("C:\\Users\\Ryan\\Desktop\\C\\Compression\\chapter.txt", "r");
    fseek(fp, 0, SEEK_END);
    unsigned long msglen = ftell(fp);
    //unsigned long msglen = 4 * KB;
    rewind(fp);
    char *msg = malloc(msglen);
    fread(msg, msglen, 1, fp);
    fclose(fp);
    char c = msg[470002];
    unsigned long encodedlen;
    unsigned char *encoded = rle_encode(msg, msglen, &encodedlen);

    fp = fopen("encoded.txt", "w");
    fwrite(encoded, encodedlen, 1, fp);
    fclose(fp);

    unsigned long decodedlen;
    unsigned char *decoded = rle_decode(encoded, encodedlen, &decodedlen);
    
    fp = fopen("decoded.txt", "w");
    fwrite(decoded, decodedlen, 1, fp);
    fclose(fp);
    
    fp = fopen("chapter.txt", "w");
    fwrite(msg, msglen, 1, fp);
    fclose(fp);

    return 0;
}