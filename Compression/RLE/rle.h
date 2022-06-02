#ifndef RLERYAN
#define RLERYAN

char* rle_encode(const char* str, unsigned long long strlen, unsigned long long *len);
char* rle_decode(const unsigned char* str, unsigned long long strlen, unsigned long long *len);

#endif