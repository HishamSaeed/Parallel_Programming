#include <stdio.h>
#include<omp.h>

// this code is the same as example 4 but differ
// having pragam omp for (without parallel)
// is if you have parallel for you must define clause
// for scheduling

int main()
{
   int num_threads=4;
   omp_set_num_threads(num_threads);
   #pragma omp parallel
   {
     	#pragma omp for
     	for (int i = 0; i < num_threads; i++)
     	{
        	#pragma omp critical
     		printf("My id is: %d !\n", omp_get_thread_num());

    	}
     }
}
