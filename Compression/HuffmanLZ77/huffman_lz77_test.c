#include <stdio.h>
#include <stdlib.h>

#include "Compression/Huffman/huffman.h"
#include "Compression/LZ77/lz77.h"

int main(){
    char *msg;
    FILE *fptr = fopen("/home/ryan/Programming/C/Compression/chapter.txt", "r");
    fseek(fptr, 0, SEEK_END);
    unsigned int flen = ftell(fptr);
    fread(msg, 1, flen, fptr);
    fclose(fptr);

    int huffencodelen, lz77encodelen;
    char *huffencode = huffman_encode(msg, &huffencodelen);
    char *lz77encode = lz77_encode(huffencode, 12, &lz77encodelen);

    fptr = fopen("/home/ryan/Programming/C/Compression/Compression/HuffmanLZ77/encoded.txt", "w");
    fwrite(lz77encode, lz77encodelen, 1, fptr);
    flcose(fptr);

    int huffdecode
    char *lz77decode = lz77_decode(lz77encode, 12, lz77encodelen);
    char *huffdecode = huffman_decode(lz77decode, )


    free(huffencode);
    free(lz77encode);
    free();
    free();
    return 0;
}