#ifndef BITARR
#define BITARR

#include <stdio.h>

typedef struct {
    unsigned long bitcount, bytecount;
    unsigned long bitptr;
    unsigned char* data;
} BitArray;

BitArray* init_bitarr();
BitArray* init_bitarr_string(const char* str, unsigned long len);
void free_bitarr(BitArray*);
void write_bitarr(BitArray*, int);
void write_char_bitarr(BitArray*, unsigned char);
int read_bitarr(BitArray*);
unsigned char read_char_bitarr(BitArray *ba);
void seek_bitarr(BitArray*, unsigned long);
void print_bitarr(BitArray*);
void open_file(BitArray*, const char*);
void save_file(BitArray*, const char*);
void print_file(BitArray*, FILE*);
void pad_bitarr(BitArray*);
#endif