#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define KB 1024
#define CHUNKSIZE (32 * KB)

typedef struct{
    const unsigned char *str;
    unsigned int strlen;
    char *encoded;
    int chunk;
} bwt_args;

void bwt_encode_chunk(bwt_args *args){
    unsigned int place;
    unsigned int start = args->chunk * CHUNKSIZE;
    unsigned int end;
    if(args->chunk > -1)
        end = (args->chunk + 1) * CHUNKSIZE;
    else
        end = args->strlen;
    
    for(unsigned int i = start; i < end; i++){
        place = 0;
        for(unsigned int j = 0; j < args->strlen; j++){
            if(i != j)
                place += compare_cycles(args->str, args->strlen, i, j);
        }
        args->encoded[place] = (i == 0 ? '\255' : args->str[i - 1]);
    }        
}

//must give string ending in \255
//find each cycles order then puts cycles final char in encoded
char *bwt_encode_multithreaded(const unsigned char *str, unsigned int strlen){
    char *encoded = malloc(strlen + 1);
    encoded[strlen] = (char)255; //end of stream char, rle adjusted to not create one

    int threadcount = 0;
    pthread_t *threads = NULL;
    bwt_args *args = NULL;
    int *retvals = NULL;

    int chunks = strlen / CHUNKSIZE;
    for(int i = 0; i < chunks; i++){
        threadcount++;
        args = realloc(args, threadcount * sizeof(bwt_args));
        args[i].chunk = i - 1;
        args[i].encoded = encoded;
        args[i].str = str;
        args[i].strlen = strlen;
        threads = realloc(threads, threadcount * sizeof(pthread_t));
        retvals = realloc(retvals, threadcount * sizeof(int));
    }

    for(int i = 0; i < chunks; i++){
        retvals[i] = pthread_create(&threads[i], NULL, bwt_encode_chunk, &args[i]);
    }

    for(int i = 0; i < chunks; i++){
        pthread_join(threads[i], NULL);
    }

    free(args);
    free(threads);
    free(retvals);
    return encoded;
}

int main(){
    FILE *msgfp = fopen("/home/ryan/Programming/C/Compression/LZ77/encoded.txt", "r");
    fseek(msgfp, 0, SEEK_END);
    unsigned int msglen = ftell(msgfp);
    rewind(msgfp);
    char *msg = malloc(msglen + 1);
    fread(msg, msglen, 1, msgfp);
    msg[msglen] = 0;
    fclose(msgfp);

    char *bwt = bwt_encode_multithreaded(msg, msglen + 1);
    free(msg);

    FILE *bwtfp = fopen("encoded.txt", "w");
    fwrite(bwt, msglen + 1, 1, bwtfp);
    fclose(bwtfp);

    bwt_decode(bwt, msglen + 1);
    bwtfp = fopen("decoded.txt", "w");
    fwrite(bwt, msglen, 1, bwtfp);
    fclose(bwtfp);

    free(bwt);

    return 0;
}