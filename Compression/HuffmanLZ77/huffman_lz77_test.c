#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Compression/Huffman/huffman.h"
#include "Compression/LZ77/lz77.h"

int main(){
    char* msg;
    FILE *fptr = fopen("/home/ryan/Programming/C/Compression/chapter.txt", "r");
    fseek(fptr, 0, SEEK_END);
    int msglen = ftell(fptr);
    rewind(fptr); 
    msg = malloc(msglen + 1);
    fread(msg, msglen, 1, fptr);
    msg[msglen] = 0;
    fclose(fptr);

    int huffencodelen, lz77encodelen;
    char *huffencode = huffman_encode(msg, strlen(msg), &huffencodelen);
    fptr = fopen("/home/ryan/Programming/C/Compression/HuffmanLZ77/huffencode.txt", "w");
    fwrite(huffencode, huffencodelen, 1, fptr);
    fclose(fptr);
    char *lz77encode = lz77_encode(huffencode, 12, huffencodelen,  &lz77encodelen);

    fptr = fopen("/home/ryan/Programming/C/Compression/HuffmanLZ77/encoded.txt", "w");
    fwrite(lz77encode, lz77encodelen, 1, fptr);
    fclose(fptr);

    char *lz77decode = lz77_decode(lz77encode, 12, lz77encodelen);
    fptr = fopen("/home/ryan/Programming/C/Compression/HuffmanLZ77/lz77decode.txt", "w");
    fwrite(lz77_decode, huffencodelen, 1, fptr);
    fclose(fptr);
    char *huffdecode = huffman_decode(lz77decode, huffencodelen);

    fptr = fopen("/home/ryan/Programming/C/Compression/HuffmanLZ77/decoded.txt", "w");
    fwrite(huffdecode, strlen(huffdecode), 1, fptr);
    fclose(fptr);

    free(huffencode);
    free(lz77encode);
    free(huffdecode);
    free(lz77decode);
    return 0;
}