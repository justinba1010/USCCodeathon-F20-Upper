#define _GNU_SOURCE //for getline()
#include "pinepipe.h"
#include <string.h>

//graphing
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


void vis_wul_graph(wul_graph* g, const char* s) {
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


void serialize_wul_graph(wul_graph g){
	for (size_t i=0; i<g.size; i++) {
		w_node* curr = (w_node*) g.vertices[i];
		while(curr != NULL) {
			printf("%u:%u", curr->neighbor, curr->weight);
			if(curr->next != NULL) {
				printf("\t");
			}
			curr=curr->next;
		}
		printf("\n");
	}
}


wul_graph deserialize_wul_graph(size_t s){
	wul_graph g = {s, (w_node**)s_calloc(s * sizeof(w_node*))};

	size_t len, parse_size;
	parse_size = 0;
	char* line, *token;
	line = NULL;

	uint32_t neighbor, weight;
	while((getline(&line, &len, stdin)) != -1){
		token = strtok(line, DELIMS);
		while(token != NULL) {
			neighbor = atoi(token);
			weight   = atoi(strtok(NULL, DELIMS));
			token    = strtok(NULL, DELIMS);
			add_neighbor(&g, parse_size, neighbor, weight);
		}
		parse_size++;
	}

	if(s != parse_size){
		fprintf(stderr, "ERROR: Parsed size and given size differ\n");
		exit(EXIT_FAILURE);
	}
	free(line);
	return g;
}

//helper
inline uint32_t min(uint32_t x, uint32_t y) {
	if (x < y) {
		return x;
	}
	return y;
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

//routing
uint32_t mbp_ewd(wul_graph* g, uint32_t s, uint32_t t) {
	if (g->size < 3) {
		fprintf(stderr, "ERROR: graph too small. MBP lacks meaning.");
		exit(EXIT_FAILURE);
	}
	if (s > g->size || t > g->size) {
		fprintf(stderr, "ERROR: Source(%u) or dest(%u) out of range(%lu)",
				s, t, g->size);
		exit(EXIT_FAILURE);
	}
	if(s == t) {
		fprintf(stderr, "ERROR: Source and dest are the same.");
		exit(EXIT_FAILURE);
	}
	/*
	 * u = unseen
	 * f = fringe
	 * i = in tree
	 */
	uint8_t*  status = (uint8_t* ) s_malloc(sizeof(uint8_t)  * g->size);
	uint32_t* weight = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);
	uint32_t* parent = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);

	for (uint32_t i = 0; i < g->size; i++) {
		status[i] = 'u';
	}
	status[s] = 'i';
	w_node* curr = (w_node*) g->vertices[s];
	uint32_t fringe_count = 0;
	while (curr != NULL) {
		status[curr->neighbor] = 'f';
		fringe_count++;
		weight[curr->neighbor] = curr->weight;
		parent[curr->neighbor] = s;
		curr = (w_node*) curr->next;
	}
	uint32_t high_fringe, high_val;
	high_fringe = 0; //prevent -Wmaybe-uninitialized under gcc 6.3.0
	while(fringe_count) {
		high_val = 0;
		for (uint32_t i=0; i < g->size; i++) {
			if (status[i] == 'f' && weight[i] > high_val) {
				/*0 is not a valid weight, so something should be higher*/
				high_fringe = i;
				high_val = weight[i];
			}
		}
		status[high_fringe] = 'i';
		fringe_count--;
		curr = (w_node*) g->vertices[high_fringe];
		while(curr != NULL ) {
			if(status[curr->neighbor] == 'u'){
				status[curr->neighbor] = 'f';
				fringe_count++;
				parent[curr->neighbor] = high_fringe;
				weight[curr->neighbor] = min(weight[high_fringe], curr->weight);
			}
			else if(status[curr->neighbor] == 'f' &&
					weight[curr->neighbor] <
					min(weight[high_fringe], curr->weight)){
				parent[curr->neighbor] = high_fringe;
				weight[curr->neighbor] = min(weight[high_fringe], curr->weight);
			}
			curr = (w_node*) curr->next;
		}
	}

	//Output
	uint32_t MB = weight[t];
/*
	//MBP from s to t
	uint32_t* res = (uint32_t*) s_malloc(g->size * sizeof(uint32_t));
	//we will rely on vmem to not use all of this.
	uint32_t curr_p, iter;
	curr_p = t;
	iter = 0;

	while(curr_p != s) {
		res[iter++] = curr_p;
		curr_p = parent[curr_p];
	}

	res[iter] = curr_p;//add source

	while(iter > 0) {
		printf("%u->", res[iter--]);
	}
	printf("%u\n", res[0]);
*/
	free(status);
	free(weight);
	free(parent);
//	free(res);
	return MB;
}

//safe standard memory management
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
