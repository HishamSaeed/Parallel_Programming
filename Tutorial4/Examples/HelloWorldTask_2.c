#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[]) {

  #pragma omp parallel
  {
    #pragma omp single
    {
      for(int t = 0; t < omp_get_num_threads();t++){
        #pragma omp task
        {
          #pragma omp critical
          printf("Hello World form task executed by thread : %d\n",omp_get_thread_num() );
        }
      }
    }

  }
  return 0;
}
