/*
 * Single file solution for the purpose of hackerrank
 */


#define _GNU_SOURCE
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMS ":\t"

typedef struct w_node{
	uint32_t neighbor;
	uint32_t weight;
	struct w_node* next;
} w_node;//weighted node

typedef struct wul_graph{
	size_t size;
	w_node** vertices;
} wul_graph;//weighted, undirected, linked list

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

inline uint32_t min(uint32_t x, uint32_t y) {
	if (x < y) {
		return x;
	}
	return y;
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
	free(status);
	free(weight);
	free(parent);
	return MB;
}

int main(int argc, char** argv) {
	//get first line
	size_t size, s, t;
	scanf("%zu%zu%zu\n", &size, &s, &t);
	wul_graph g = deserialize_wul_graph(size);
	printf("%u\n", mbp_ewd(&g, s, t));
	destroy_wul_graph(&g);
	return EXIT_SUCCESS;
}

