#include "familytree.h"
#include <omp.h>

#define T 25 // THRESHOLD

int traverse_seq(tree *node, int num_threads) {
    if (node == NULL) return 0;

    int father_iq, mother_iq;

    father_iq = traverse_seq(node->father, num_threads);
    mother_iq = traverse_seq(node->mother, num_threads);

    node->IQ = compute_IQ(node->data, father_iq, mother_iq);
    genius[node->id] = node->IQ;
    return node->IQ;
}

int traverse_par(tree *node,int num_threads){
	if(node == NULL) return 0;

	int father_iq,mother_iq;

	#pragma omp task shared(father_iq) //final(node->id > 8000)
	father_iq = traverse_par(node->father, num_threads);
	//printf("node id = %d\n",node->id );
	#pragma omp task shared(mother_iq) //final(node->id > 8000)
	mother_iq = traverse_par(node->mother, num_threads);

	#pragma omp taskwait
	node->IQ = compute_IQ(node->data, father_iq, mother_iq);
	genius[node->id] = node->IQ;
	return node->IQ;
}

int traverse(tree *node, int num_threads){
	int node_iq;
	omp_set_nested(1);
	omp_set_num_threads(num_threads);

	#pragma omp parallel
	{
		#pragma omp single firstprivate(node,num_threads)
		node_iq = traverse_par(node,num_threads);

	}
	return node_iq;
}
