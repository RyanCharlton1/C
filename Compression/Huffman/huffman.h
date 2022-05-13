#ifndef HUFFMANMANRYAN
#define HUFFMANMANRYAN

#define EOT 4
#define LEFT 2
#define RIGHT 1

char* huffman_encode(const char *str, unsigned long int *len);
char* huffman_decode(const char *str, unsigned int *len, int unsigned msglen);

#endif