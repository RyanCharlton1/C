#include "ECC/CRC/crc.h"

#include <stdlib.h>
#include <string.h>

int block_empty(unsigned char *block, int blocksize){
    for(int i = 0; i < blocksize; i++)
        if(block[i] > 0)
            return 0;
    
    return 1;
}

//calculates 8 bit crc considering bytes to start on right most bit
uint8_t CRC8(unsigned char *block, uint8_t blocksize, uint8_t poly){
    unsigned char *blockcpy = malloc(blocksize + 1);
    memcpy(blockcpy, block, blocksize);
    
    uint8_t MSblock;
    while(!block_empty(blockcpy, blocksize)){
        //find most significant non 0 block
        for(MSblock = 0; !blockcpy[MSblock]; MSblock++){}

        //find how much to shift poly by to fit most significant 1 
        int shift = 0;
        for(unsigned char c = blockcpy[MSblock]; !(c & 128); c <<= 1)
            shift++;
        
        uint8_t polymsb = poly;
        uint8_t polylsb = poly;

        polymsb >>= shift;
        polylsb <<= 8 - shift;
        polylsb += 128 >> shift;

        //divide(xor) by polynomial shifted to fit two bytes
        blockcpy[MSblock] ^= polymsb;
        blockcpy[MSblock + 1] ^= polylsb;
    }
    
    uint8_t result = blockcpy[blocksize];
    result >> 1;

    free(blockcpy);
    return result;
}