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

    return 0;
}