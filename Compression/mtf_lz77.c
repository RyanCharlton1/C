#include "Compression/MTF/mtf.h"
#include "Compression/LZ77/lz77.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *msgfp = fopen("chapter.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    unsigned int msglen = ftell(msgfp);
    rewind(msgfp);

    char *msg = malloc(msglen);
    fread(msg, msglen, 1, msgfp);
    fclose(msgfp);

    unsigned int lz77len, mtflen;

    char *mtfencoded = mtf_encode(msg, msglen);
    char *lz77encoded = lz77_encode(mtfencoded, 12, msglen, &lz77len);

    msgfp = fopen("encoded.txt", "w");
    fwrite(lz77encoded, lz77len, 1, msgfp);
    fclose(msgfp);

    char *lz77decoded = lz77_decode(lz77encoded, 12, lz77len, &mtflen);
    char *mtfdecoded = mtf_decode(lz77decoded, mtflen);

    msgfp = fopen("decoded.txt", "w");
    fwrite(mtfdecoded, mtflen, 1, msgfp);
    fclose(msgfp);

    free(mtfencoded);
    free(mtfdecoded);
    free(lz77decoded);
    free(lz77encoded);
    return 0;
}