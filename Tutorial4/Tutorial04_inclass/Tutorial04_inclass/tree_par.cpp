#include <iostream>
#include<omp.h>
#include<unistd.h>

struct node
{
	struct node *left, *right;
	int key;
	node(int k): key(k){}
};

void process(struct node *p){
        usleep(1000000);
	#pragma omp critical
        {std::cout<< "element with key: "<<p->key << "is processed"<<std::endl;}
}

void traverse(struct node *p)
{   	
	#pragma omp parallel
	{
	  #pragma omp sections
	  {
	      #pragma omp section
	      {
		if ( p->left )
	          traverse( p->left );
	      }
	      #pragma omp section
	      {
		if ( p->right )
	   	  traverse( p-> right );
	      }
	   }
	}
	process(p);
}


int main(int argc, char *argv[])
{
	struct node *tree = new struct node(0); 
	tree->left = new struct node(1);
	tree->right = new struct node(2);
	tree->left->left = new struct node(3); 
	tree->left->right = new struct node(4); 
	tree->right->left = new struct node(5); 
	tree->right->right = new struct node(6); 
	
	omp_set_num_threads(2);
	omp_set_nested(1);
	omp_set_max_active_levels(2);

	traverse(tree); 
	
	delete tree->right->right;
	delete tree->right->left;
	delete tree->left->right;
	delete tree->left->left;
	delete tree->right;
	delete tree->left;
	delete tree;
	return 0;
}
