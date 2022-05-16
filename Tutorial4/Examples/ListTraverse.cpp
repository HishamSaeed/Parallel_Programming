#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <forward_list>

void process_element(int &elem){
  usleep(1000000);
  printf("%d\n",elem );
}

void traverse_list(std::forward_list<int> &l){

  for(auto it = l.begin(); it != l.end(); it++){
    process_element(*it);
  }
}

int main(int argc, char const *argv[]) {
  std::forward_list<int> l;
  l.assign({0,1,2,3,4,5,6,7,8,9});

  traverse_list(l);
  return 0;
}
