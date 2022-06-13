#ifndef LZ77RYAN
#define LZ77RYAN

char* lz77_encode(const char* str, int windowbits, unsigned int strlen, unsigned int *len);
char* lz77_decode(const unsigned char* str, int windowbits, unsigned int strlen, unsigned int *len);
#endif