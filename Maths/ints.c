#include "Maths/ints.h"

unsigned long powi(unsigned long x, unsigned long y){
    if(y)
        return x * powi(x, y - 1);
    else
        return 1;
}

int log2i(unsigned long x){
    if(x == 1)
        return 0;
    else
        return 1 + log2i(x >> 1);
}