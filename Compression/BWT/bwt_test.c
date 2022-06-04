#include <stdio.h>
#include <string.h>

#include "Compression/BWT/bwt.h"

int main(){
    const char *msg = "^BANANA\255";
    char *bwt = bwt_encode(msg, strlen(msg));

    for(int i = 0; i < strlen(msg); i++)
        putchar(bwt[i]);
    putchar('\n');
    
    free(bwt);
    return 0;
}