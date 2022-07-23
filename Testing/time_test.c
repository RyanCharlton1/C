//#include <stdio.h>
//#include <time.h>
//#include <unistd.h>
//
//struct test{
//    int x;
//    char c;
//};
//
//int main(){
//    sizeof(struct test);
//    clock_t start, end;
//    start = clock();
//    printf("time : %ld\n", start);
//    //for(int i = 0; i < 1000000; i++)
//    sleep(2);
//    end = clock();
//    printf("time : %lf\n", (end - start) / (double)CLOCKS_PER_SEC);
//}

#include <stdio.h>
#include <time.h>
void take_enter() {
   printf("Press enter to stop the counter \n");
   while(1) {
      if (getchar())
      break;
   }
}
main() {
   // Calculate the time taken by take_enter()
   clock_t t;
   t = clock();
   printf("Timer starts\n");
   take_enter();
   printf("Timer ends \n");
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
   printf("The program took %f seconds to execute\n", time_taken);
}