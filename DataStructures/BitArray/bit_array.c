#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DataStructures/BitArray/bit_array.h"

BitArray* init_bitarr(){
    return (BitArray*)calloc(1, sizeof(BitArray));
}

BitArray* init_bitarr_string(const char* str){
    BitArray* ba = (BitArray*)calloc(1, sizeof(BitArray));
    int n = strlen(str);
    ba->data = malloc(n);
    memcpy(ba->data, str, n);
    ba->bytecount = n;
    ba->bitcount = n * 8;
    return ba;
}

void free_bitarr(BitArray *ba){
    free(ba->data);
    free(ba);
}

void write_bitarr(BitArray* ba, int n){
    int x = ba->bitcount % 8;
    if(x == 0){
        ba->bytecount++;
        ba->data = (unsigned char*)realloc(ba->data, ba->bytecount);
        ba->data[ba->bytecount - 1] = 0;
    }

    if(n){
        ba->data[ba->bytecount - 1] += 1 << (7 - x);
    }

    ba->bitcount++;
}

void write_char_bitarr(BitArray *ba, unsigned char c){
    for(int i = 0; i < 8; i++){
        write_bitarr(ba, c & (128 >> i));
    }
}

int read_bitarr(BitArray* ba){
    unsigned char c = ba->data[ba->bitptr / 8];
    int n = ba->bitptr % 8;
    int mask = 1;
    for(int i = 0; i < 7 - n; i++)
        mask *= 2;

    ba->bitptr++;
    return c & mask;
}

unsigned char read_char_bitarr(BitArray *ba){
    unsigned char c = 0;
    for(int i = 0; i < 8; i++)
        if(read_bitarr(ba))
            c += 128 >> i;
    
    return c;
}

void seek_bitarr(BitArray *ba, unsigned int pos){
    ba->bitptr = pos;
}

void print_bitarr(BitArray *ba){
    for(int i = 0; i < ba->bytecount; i++){
        for(int j = 7; j >= 0; j--){
            if(ba->data[i] & (1 << j))
                printf("1");
            else
                printf("0");
        }
        printf(" ");
    }
    printf("\n");
}

void open_file(BitArray *ba, const char* path){
    FILE *fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    ba->bytecount = ftell(fp);
    rewind(fp);

    ba->data = (char*)malloc(ba->bytecount * sizeof(char));
    fread(ba->data, ba->bytecount, 1, fp);
    fclose(fp);
    seek_bitarr(ba, 0);
}

void save_file(BitArray* ba, const char* path){
    FILE *fp = fopen(path, "w");

    for(int i = 0; i < ba->bytecount; i++)
        fwrite(ba->data + i, 1, 1, fp);

    fclose(fp);
}

void print_file(BitArray *ba, FILE *fp){
    for(int i = 0; i < ba->bytecount; i++)
        fwrite(ba->data + i, 1, 1, fp);
}

void pad_bitarr(BitArray *ba){
    if(ba->bitcount % 8)
        return;
    
    ba->bytecount++;
    ba->data = (unsigned char*)realloc(ba->data, ba->bytecount);
    ba->data[ba->bytecount - 1] = 0;
    ba->bitcount = 8 * ba->bytecount;
}

/*
int main(){
    BitArray *ba = init_bitarr();
    write_bitarr(ba, 1);
    write_bitarr(ba, 0);
    write_bitarr(ba, 0);
    write_bitarr(ba, 0);
    write_bitarr(ba, 1);
    write_bitarr(ba, 0);
    write_bitarr(ba, 1);
    write_bitarr(ba, 1);

    write_bitarr(ba, 0);
    write_bitarr(ba, 1);
    write_bitarr(ba, 0);
    write_bitarr(ba, 1);

    print_bitarr(ba);

    seek_bitarr(ba, 0);
    for(int i = 0; i < ba->bitcount; i++){
        int n = read_bitarr(ba);
        printf("%d\n", n);
    }

    save_file(ba, "/home/ryan/Documents/test.txt");
    free_bitarr(ba);
    return 0;
}*/
