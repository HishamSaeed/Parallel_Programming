#include <stdio.h>
#include <omp.h>
#include<unistd.h>

struct node
{
  struct node *left, *right;
  int key;
  node(int k): key(k){}
};

void process(struct node *p){
  usleep(1000000);
  printf("element with key: %d is processed,from thread: %d\n",p->key,omp_get_thread_num());
}

void traverse(struct node *p){

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        if(p->left != NULL)
            traverse(p->left);
      }
      #pragma omp section
      {
        if (p->right != NULL)
            traverse(p->right);
      }
    }
  }



  process(p);
}

int main(int argc, char const *argv[]) {
  struct node *tree = new struct node(0);
  tree->left = new struct node(1);
  tree->right = new struct node(2);
  tree->left->left = new struct node(3);
  tree->right->left = new struct node(4);
  tree->left->right = new struct node(5);
  tree->right->right = new struct node(6);

  omp_set_nested(1);
  omp_set_max_active_levels(2);

  traverse(tree);
  return 0;
}
