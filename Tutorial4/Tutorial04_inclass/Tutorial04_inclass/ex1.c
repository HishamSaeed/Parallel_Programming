//#include <iostream>
#include <stdio.h>
#include<omp.h>

 int main()
 {

   int num_threads=4;
   omp_set_num_threads(num_threads);
   for (int i = 0; i < num_threads; i++)
   {
      printf("My id is: %d !\n", omp_get_thread_num());
   }
   return 0;
 }
