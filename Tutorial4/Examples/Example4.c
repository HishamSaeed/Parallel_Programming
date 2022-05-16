#include <stdio.h>
#include <omp.h>

// The problem with this code is that for has no clause
// only one thread executing for loop 4 times

int main(int argc, char const *argv[]) {
  int num_threads = 4;
  omp_set_num_threads(num_threads);


  #pragma omp parallel
  {
    #pragma omp parallel for
    for(int i = 0; i < num_threads; i++){
      #pragma opm critical
      printf("My id is: %d\n",omp_get_thread_num() );
    }
  }

  return 0;
}
