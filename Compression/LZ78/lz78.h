#ifndef LZ78RYAN
#define LZ78RYAN

#define EOT 4

typedef struct{
    unsigned char index;
    char c;
    char* str;
} Entry;

typedef Entry* Dict;

char* lz78_encode(const char* str, int* len);
char* lz78_decode(const char* str, int strlen, int* len);

#endif