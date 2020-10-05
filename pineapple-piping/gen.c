#include "gen.h"

void vis_wul_graph(const char* s, wul_graph* g) {
	FILE *fptr = fopen(s, "w");
	if (fptr == NULL) {
		fprintf(stderr, "ERROR: Cannot open file");
		return;
	}
	w_node* curr;
	fprintf(fptr, "graph {\n");
	for(uint32_t i=0; i < g->size; i++) {

		curr = (w_node*) g->vertices[i];
		while (curr != NULL) {
#ifndef UNDIR
			//only show one direction if in release mode
			if (i < curr->neighbor)
#endif
			{
				fprintf(fptr, "\t%u -- %u[label=\"  %u\"];\n", i,
						curr->neighbor, curr->weight);
			}
			curr = (w_node*) curr->next;
		}
	}
	fprintf(fptr, "}\n");
	fclose(fptr);
	return;
}


void shuffle(uint32_t* arr, size_t size) {
	uint32_t tmp, r;
	for (uint32_t i = 0; i < size; i++) {
		r = rand() % size;
		tmp = arr[r];
		arr[r] = arr[i];
		arr[i] = tmp;
	}
	return;
}


void add_neighbor(wul_graph* g, uint32_t i, uint32_t j, uint32_t weight) {
	//adds connection from i to j
	w_node* new  = (w_node*) s_malloc( sizeof(w_node) );
	new->neighbor = j;
	new->weight = weight;
	new->next = g->vertices[i];
	g->vertices[i] = new;
	return;
}


void serialize_wul_graph(wul_graph g){
	for (size_t i=0; i<g.size; i++) {
		w_node* curr = (w_node*) g.vertices[i];
		while(curr != NULL) {
			printf("%u:%u\t", curr->neighbor, curr->weight);
			curr=curr->next;
		}
		printf("\n");
	}
}


bool is_connected (wul_graph* g, uint32_t index, uint32_t val) {
	w_node* curr = (w_node*) g->vertices[index];
	while (curr !=NULL) {
		if (curr->neighbor == val) {
			return true;
		}
		curr = (w_node*) curr->next;
	}
	return false;
}


wul_graph* gen_wul_graph(size_t size, uint32_t deg) {
	//won't work for odd size and degree
	if(deg >= size) {
		fprintf(stderr, "ERROR: deg >= size. function not appropriate for this"
				" use case.\n");
		exit(EXIT_FAILURE);
	}
	//attempt perfect average degree
	uint32_t num_edges;
	if ( ! ( size % 2 ) ) {
		num_edges = (size/2)* deg;
	}
	else if ( ! ( deg % 2 )) {
		num_edges = (deg/2)* size;
	}
	else{
		num_edges = (deg/2)* size;
		fprintf(stderr, "ERROR: deg and size are odd. Results may be "
				"unexpected.\n");
		exit(EXIT_FAILURE);
	}
	wul_graph* g = (wul_graph*) s_malloc((size) * sizeof(wul_graph));
	g->size = size;
	g->vertices = (w_node**) s_malloc(sizeof(w_node*) * size);
	//size many pointers to node structs
	
	for (uint32_t i = 0; i < size; i++) {
		g->vertices[i] = NULL;
	}//ensure all neighbor lists are initially NULL

	uint32_t* init_conn = (uint32_t*)s_malloc(sizeof(uint32_t) * size);
	for (uint32_t i=0; i < size; i++) {
		init_conn[i] = i;
	}//create initial ordering.
	shuffle(init_conn, size);

	uint32_t new_weight;
	for (uint32_t i=0; i < size-1; i++) {
		new_weight = rand() % W_MAX + 1;
		add_neighbor(g, init_conn[i], init_conn[i+1], new_weight);
		add_neighbor(g, init_conn[i+1], init_conn[i], new_weight);
	}//ensure randomized connectivity
	new_weight = rand() % W_MAX + 1;
	add_neighbor(g, init_conn[0], init_conn[size-1], new_weight);
	add_neighbor(g, init_conn[size-1], init_conn[0], new_weight);
	free(init_conn);

	uint32_t x, y;
	for (uint32_t i = size; i< num_edges; i++) {//set at current number of vertices
		do {
			x = rand() % size;
			y = rand() % size;
		}
		while(is_connected(g, x, y) || x == y);
			new_weight = rand() % W_MAX + 1;
			add_neighbor(g, x, y, new_weight);
			add_neighbor(g, y, x, new_weight);
	}
	return g;
}


void destroy_wul_graph(wul_graph* g) {
	//free all nodes in array
	w_node* curr;
	w_node* curr_f;
	for(size_t i=0; i < g->size; i++) {
		curr = (w_node*) g->vertices[i];
		while (curr != NULL) {
			curr_f = curr;
			curr = (w_node*) curr->next;
			free(curr_f);
		}
	}
	free(g->vertices);
	return;
}


int main(int argc, char** argv) {
	if(argc != 5 && argc != 6) {
		fprintf(stderr, "Usage: %s <size> <degree> <s> <t> [vis filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	size_t size = atoi(argv[1]);
	size_t deg  = atoi(argv[2]);
	size_t s    = atoi(argv[3]);
	size_t t    = atoi(argv[4]);
	if(s >= size || t >= size || s==t) {
		fprintf(stderr, "ERROR: s or t too small, or equal to each other\n");
		exit(EXIT_FAILURE);
	}

	wul_graph* g = gen_wul_graph(size, deg);

	printf("%zu\t%zu\t%zu\n", size, s, t);
	serialize_wul_graph(*g);

	if (argc == 6){
	    vis_wul_graph(argv[5], g);
	}
	destroy_wul_graph(g);
	free(g);
	return EXIT_SUCCESS;
}


void* s_malloc(size_t size) {
	void* ptr;
	if((ptr = malloc(size))) {
		return ptr;
	}
	else {
		fprintf(stderr, "ERROR: Failed to malloc\n");
		exit(EXIT_FAILURE);
	}
}


void* s_calloc(size_t size) {
	void* ptr;
	if((ptr = calloc(size, 1))) {
		return ptr;
	}
	else {
		fprintf(stderr, "ERROR: Failed to calloc\n");
		exit(EXIT_FAILURE);
	}
}


void* s_realloc(void* ptr, size_t size) {
	//okay to reassign ptr, as we exit on failure
	if((ptr = realloc(ptr, size))){
		return ptr;
	}
	else {
		fprintf(stderr, "ERROR: Failed to realloc\n");
		exit(EXIT_FAILURE);
	}
}
