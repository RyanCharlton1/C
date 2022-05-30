#ifndef LZMARYAN
#define LZMARYAN

char* lzma_encode(const char *str, int strlen, int *msglen);
char* lzma_decode(const char *str, int strlen);

#endif