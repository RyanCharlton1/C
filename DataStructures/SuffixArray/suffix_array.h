#ifndef SUFFIXARRRYAN
#define SUFFIXARRRYAN

typedef struct{
    char *str;
    unsigned int size;
    unsigned int *indices;
} SuffixArray;


SuffixArray *init_suffixarr(char *str, unsigned int size);
void free_suffixarr(SuffixArray *sa);
void print_suffixarr_int(SuffixArray *sa);
void print_suffixarr_char(SuffixArray *sa);
void bwt_encode_suffixarr(SuffixArray *sa);


#endif