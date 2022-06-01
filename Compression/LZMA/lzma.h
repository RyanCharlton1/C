#ifndef LZMARYAN
#define LZMARYAN

#define EOT 4

#define LITERAL     0
#define MATCH       10
#define SHORTREP    1100
#define LONGREP0    1101
#define LONGREP1    1110
#define LONGREP2    11110
#define LONGREP3    11111

#define DICTSIZE    4294967296
#define MAXRUN      273


char* lzma_encode(const char *str, unsigned long strlen, unsigned long *msglen, int windowbits);
char* lzma_decode(const char *str, unsigned long strlen);

#endif