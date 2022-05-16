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
        std::cout<< "element with key: "<<p->key << "is processed"<<std::endl;
}

void traverse(struct node *p){
	if (p->left != NULL)
	   traverse(p->left);
	if (p->right != NULL)
	   traverse(p-> right);

	process(p);
}


int main(int argc, char *argv[]){

	struct node *tree = new struct node(0); 
	tree->left = new struct node(1);
	tree->right = new struct node(2);
	tree->left->left = new struct node(3); 
	tree->left->right = new struct node(4); 
	tree->right->left = new struct node(5); 
	tree->right->right = new struct node(6); 

	traverse(tree); 

	return 0;
}
