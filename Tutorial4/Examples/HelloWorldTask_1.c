#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[]) {

  #pragma omp parallel
  {
    #pragma omp task
    {
      #pragma omp critical
      printf("Hello World form task executed by thread : %d\n",omp_get_thread_num() );
    }
  }
  return 0;
}
