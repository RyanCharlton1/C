#ifndef LZ77RYAN
#define LZ77RYAN

char* lz77_encode(const char* str, int windowbits, int *len);
char* lz77_decode(const char* str, int windowbits, int len);
#endif