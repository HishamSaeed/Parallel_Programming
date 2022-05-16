#include <stdio.h>
#include <omp.h>

// The problem with this code is that it does not parallelise
// for loop there is no pragma for, hence the for loop is executed by each thread
// the question is why it give same output everytime

int main(int argc, char const *argv[]) {
  int num_threads = 4;
  omp_set_num_threads(num_threads);

  #pragma omp parallel
  {
    for(int i = 0; i < num_threads; i++){
      #pragma opm critical
      printf("My id is: %d\n",omp_get_thread_num() );
    }
  }

  return 0;
}
