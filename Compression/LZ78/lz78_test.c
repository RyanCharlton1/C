#include "Compression/LZ78/lz78.h"

int main(){
    const char *msg = "abaabbaaabbbaaaabbbbaaaaabbbbb";
    
    int encodedlen;
    char *encoded = lz78_encode(msg, &encodedlen);

    printf("%s\n", msg);
    for(int i = 0; i < encodedlen; i += 2)
        printf("%d%c", encoded[i], encoded[i+1]);
    putchar('\n');
    
    int decodedlen;
    char *decoded = lz78_decode(encoded, encodedlen, &decodedlen);
    printf("%s\n", decoded);

    free(encoded);
    free(decoded);
    return 0;
}