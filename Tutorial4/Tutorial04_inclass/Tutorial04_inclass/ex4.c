#include <stdio.h>
#include<omp.h>

int main()
{
   int num_threads=4;
   omp_set_num_threads(num_threads);
   #pragma omp parallel 
   {
     	#pragma omp parallel for
     	for (int i = 0; i < num_threads; i++) 
     	{
        	//#pragma omp critical
     		printf("My id is: %d !\n", omp_get_thread_num());

    	}
     }
}
