#include <stdio.h>
#include <time.h>

struct test{
    int x;
    char c;
};

int main(){
    sizeof(struct test);
    clock_t now;
    now = clock();
    printf("time : %ld\n", now);
    //for(int i = 0; i < 1000000; i++)
    now = clock();
    printf("time : %ld\n", now);
}