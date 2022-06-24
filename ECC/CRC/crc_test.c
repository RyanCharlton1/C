#include "ECC/CRC/crc.h"
#include <stdlib.h>

int main(){
    char *c = malloc(1);
    c[0] = 't';
    //c[1] = 'e';

    uint8_t crc1 = CRC8(c, 1, 193);
    c[0] = 's';
    uint8_t crc2 = CRC8(c, 1, 193);

    free(c);
}