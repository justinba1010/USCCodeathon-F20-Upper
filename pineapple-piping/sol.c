#include "proj.h"

uint32_t get_connect_prob(uint32_t numer, uint32_t denom) {
/* Integer based probability function, returns 1 if success, with probability
 * numer/denom. Otherwise, return 0.
 */
	if ( numer == denom ) {
		return 1;
	}
	else if( numer > denom) {
		assert(0);
	}
	else {
		uint32_t scale, r, s_numer, s_denom;
		scale   = (RAND_MAX / denom);
		s_denom = denom * scale;
		s_numer = numer * scale;
		do {
			r = rand();
		} while(r >= s_denom);
		/* Could loop forever, but worst prob is 0.5 */

		if( r < s_numer ) {
			return 1;
		}
		else {
			return 0;
		}
	}
}


void vis_wul_graph(struct tm* tm, wul_graph* g) {
	static unsigned char count = 0;
	char f_name[29];
	FILE *fptr;

#ifdef REFRESH
	sprintf(f_name, "test.dot");
	//prevent unused variable warnings for count and tm
	count++;
	count = tm->tm_year;
#else
	sprintf (f_name, "%04d-%02d-%02d__%02d-%02d-%02d-%u.dot",
	tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
	tm->tm_hour, tm->tm_min, tm->tm_sec, count++);
#endif
	fptr = fopen(f_name, "w");
	if (fptr == NULL) {
		fprintf(stderr, "Cannot open file");
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
				fprintf(fptr, "\t%u -- %u[label=\" %u\"];\n", i,
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
		r = u_rand(size);
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
	g->vertices[i] = (w_node*) new;
	return;
}
void serialize_wul_graph(wul_graph g){
	printf("%u\n", g.size);
	for (size_t i=0; i<g.size; i++) {
		w_node* curr = (w_node*) g.vertices[i];
		while(curr != NULL) {
			printf("%u:%u\t", curr->neighbor, curr->weight);
			curr=curr->next;
		}
		printf("\n");
	}
}

wul_graph deserialize_wul_graph(){
	wul_graph g;
	g.size = sscanf("%d\n");


	return g;
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
wul_graph* gen_graph_one(size_t size, uint32_t deg) {
	//won't work for odd size and degree
	if(deg == 1) {
		fprintf(stderr, "deg of 1 is too small.\n");
		exit(EXIT_FAILURE);
	}
	if(deg >= size) {
		fprintf(stderr, "deg >= size. function not appropriate for this"
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
		fprintf(stderr, "deg and size are odd. Results may be "
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
		new_weight = u_rand(W_MAX) + 1;
		add_neighbor(g, init_conn[i], init_conn[i+1], new_weight);
		add_neighbor(g, init_conn[i+1], init_conn[i], new_weight);
	}//ensure randomized connectivity
	new_weight = u_rand(W_MAX) + 1;
	add_neighbor(g, init_conn[0], init_conn[size-1], new_weight);
	add_neighbor(g, init_conn[size-1], init_conn[0], new_weight);

	uint32_t x, y;
	for (uint32_t i = size; i< num_edges; i++) {//set at current number of vertices
		do {
			x = u_rand(size);
			y = u_rand(size);
		}
		while(is_connected(g, x, y) || x == y);
			new_weight = u_rand(W_MAX) + 1;
			add_neighbor(g, x, y, new_weight);
			add_neighbor(g, y, x, new_weight);
	}
	return g;

}

wul_graph* gen_graph_two(size_t size, uint32_t numer, uint32_t denom) {
	/*Assumes that the prob +1 ~= prob*/
	wul_graph* g = (wul_graph*) s_malloc((size) * sizeof(wul_graph));
	g->size = size;
	g->vertices = (w_node**) s_malloc(sizeof(w_node*) * size);
	//size many pointers to node structs

	for (uint32_t i = 0; i < size; i++) {
		g->vertices[i] = NULL;
	}//ensure all neighbor lists are initially NULL

	uint32_t* init_conn = (uint32_t*)s_malloc(sizeof(uint32_t) * size);
	uint32_t* init_conn_pos = (uint32_t*)s_malloc(sizeof(uint32_t) * size);
	for (uint32_t i=0; i < size; i++) {
		init_conn[i] = i;
	}//create initial ordering.
	shuffle(init_conn, size);
	uint32_t new_weight;
	for (uint32_t i=0; i < size-1; i++) {
		new_weight = u_rand(W_MAX) + 1;
		add_neighbor(g, init_conn[i], init_conn[i+1], new_weight);
		add_neighbor(g, init_conn[i+1], init_conn[i], new_weight);
	}//ensure randomized connectivity
	new_weight = u_rand(W_MAX) + 1;
	add_neighbor(g, init_conn[0], init_conn[size-1], new_weight);
	add_neighbor(g, init_conn[size-1], init_conn[0], new_weight);


	for (uint32_t i=0; i < size; i++) {
		init_conn_pos[init_conn[i]] = i;
	}//store positions to prevent connection lookups

/*
	printf("init_conn:\n");
	for (int i=0; i<size-1; i++) {
		printf("%u..", init_conn[i]);
	}
	printf("%u\n", init_conn[size-1]);

	printf("init_conn_pos:\n");
	for (int i=0; i<size-1; i++) {
		printf("%u..", init_conn_pos[i]);
	}
	printf("%u\n", init_conn_pos[size-1]);
*/

//	printf("size: %u\n", size);

/*
	for(int i =0; i < size; i++) {
		for(int j=0; j<size; j++) {
			if(is_connected(g, i, j)){
				printf("i:%u   j:%u\n", i, j);
			}
		}
	}
*/

	uint32_t exist_next, exist_prev;
	for (uint32_t i=0; i < size; i++) {
		if(init_conn_pos[i] == size-1 || init_conn_pos[i] == 0){
			if(init_conn_pos[i] == 0) {
				exist_next = init_conn[1];
				exist_prev = init_conn[size-1];
				
			}
			else {
				exist_next = init_conn[0];
				exist_prev = init_conn[size-2];
			}
		}
		else {
			exist_next = init_conn[init_conn_pos[i]+1];
			exist_prev = init_conn[init_conn_pos[i]-1];
		}



		for (uint32_t j = i+1; j <size; j++) {
	/*
		for (int j = 0; j <size; j++) {
			if(j == exist_next || j == exist_prev) {
				printf("i:%u   j:%u\n", i, j);
			}
	*/
			if( j != exist_prev && j != exist_next && get_connect_prob( numer, denom ) ) {
				new_weight = u_rand(W_MAX) + 1;
				add_neighbor(g, i, j, new_weight);
				add_neighbor(g, j, i, new_weight);
			}
		}
	}
	return g;
}


void destroy_wul_graph(wul_graph* g) {
	//free all nodes in array
	w_node* curr;
	w_node* curr_f;
	for(uint32_t i=0; i < g->size; i++) {
		curr = (w_node*) g->vertices[i];
		while (curr != NULL) {
			curr = (w_node*) curr->next;
			curr_f = curr;
			free(curr_f);
		}
		free(g->vertices);
	}
	return;
}


int main(void) {
#ifdef RAND
	time_t rand_t;
	srand((unsigned) time(&rand_t));

	printf("%lu\n",rand_t);
	//1601697874
#endif



	time_t now;
	//struct tm *tm;
	now = time(0);
	if ((tm = localtime (&now)) == NULL) {
		printf ("Error extracting time stuff\n");
		return EXIT_FAILURE;
	}
	wul_graph* one = gen_graph_one(SIZE, G_ONE_DEG);
	//wul_graph* one = gen_graph_two(SIZE, G_TWO_NUMER, G_TWO_DENOM);
	mbp_ewd(one, 4, 5);
	mbp_ewd_heap(one, 4, 5);
	serialize_wul_graph(*one);


	if (SIZE <= VIS_SIZE_MAX) {
		vis_wul_graph(tm, one);
	}

/*
	uint32_t x, y;
	wul_graph* one, *two;
	for (uint32_t i=0; i < RUNS ; i++) {
		one   = gen_graph_one(SIZE, G_ONE_DEG);
		two   = gen_graph_two(SIZE, G_TWO_NUMER, G_TWO_DENOM);
		for(uint32_t j = 0; j < RUNS; j++) {
			do {
				x = u_rand(SIZE);
				y = u_rand(SIZE);
			}while (x == y);
			mbp_ewd     (one, x, y);
			mbp_ewd_heap(one, x, y);
			//mbp_kruskal (one_one, x, y);

			mbp_ewd     (two, x, y);
			mbp_ewd_heap(two, x, y);
			printf("---next pair of vertices---\n");
		//mbp_kruskal (two_one, x, y);
		}
		//destroy_wul_graph(one);
		//destroy_wul_graph(two);
			printf("---next pair of graphs---\n");
	}
*/
	return EXIT_SUCCESS;
}

heap* gen_heap(size_t cap) {
	heap* h = (heap*) s_malloc(sizeof(heap));
	h->size = 0;
	h->cap = cap;
	h->H = (edge*) s_malloc(sizeof(edge) * (cap + 1));
	return h;
}


edge heap_max(heap* h) {
	return h->H[1];
}


void heap_insert(heap* h, edge e) {
	if(h->size == h->cap) {
		h->H = s_realloc(h->H, h->cap*2*sizeof(edge));
		h->cap *= 2;
	}
	h->H[++h->size] = e;
	uint32_t i = h->size;
	edge tmp;
	while(i>= 2 && h->H[i/2].wt < h->H[i].wt) {
		tmp       = h->H[i/2];
		h->H[i/2] = h->H[i];
		h->H[i]   = tmp;
		i /= 2;
	}
}


void heap_del(heap* h, uint32_t del_index) {
	if(del_index == 0) {
		fprintf(stderr, "Heap is 1 indexed.\n");
		exit(EXIT_FAILURE);
	}
	if(h->size == 0) {
		fprintf(stderr, "WARN: ignoring deletion from empty heap.\n");
		return;
	}
	h->H[del_index] = h->H[h->size--];
	uint32_t i = del_index;
	edge tmp;
	if(i >= 2 && h->H[i].wt < h->H[i/2].wt) {
		while(i >= 2 && h->H[i/2].wt > h->H[i].wt) {
			tmp      = h->H[i/2];
			h->H[i/2] = h->H[i];
			h->H[i]   = tmp;
			i /= 2;
		}
	}
	else {
		while((2*i <= h->size &&  h->H[i].wt < h->H[2*i].wt) ||
				(2*i+1 <= h->size && h->H[i].wt < h->H[2*i + 1].wt)) {
			if(2*i == h->size || h->H[2*i].wt > h->H[2*i +1].wt) {
				tmp       = h->H[2*i];
				h->H[2*i] = h->H[i];
				h->H[i]   = tmp;
				i *= 2;
			}
			else {
				tmp           = h->H[2*i + 1];
				h->H[2*i + 1] = h->H[i];
				h->H[i]       = tmp;
				i = 2*i + 1;
			}
		}
	}
	return;
}


void vis_heap(heap* h) {
	for(uint32_t i=1; i < h->size; i++) {
		printf("(s:%u-t:%u-w%u)..", h->H[i].src, h->H[i].dst, h->H[i].wt);
	}
	if (h->size != 0) {
		printf("(s:%u-t:%u-w:%u)\n", h->H[h->size].src,
				h->H[h->size].dst, h->H[h->size].wt);
	}
	return;
}


void mbp_ewd(wul_graph* g, uint32_t s, uint32_t t) {
	if (s > g->size || t > g->size) {
		fprintf(stderr, "Source(%u) or dest(%u) out of range(%lu)",
				s, t, g->size);
		exit(EXIT_FAILURE);
	}
	if(s == t) {
		fprintf(stderr, "Source and dest are the same.");
	}
	/*
	 * u = unseen
	 * f = fringe
	 * i = in tree
	 */
	uint8_t*  status = (uint8_t* ) s_malloc(sizeof(uint8_t)  * g->size);
	uint32_t* weight = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);
	uint32_t* parent = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);

	clock_t time;
	time = clock();

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
	while(fringe_count) {
		high_val = 0;
		for (uint32_t i=0; i < g->size; i++) {
			if (status[i] == 'f' && weight[i] > high_val) {
				/*0 is not a valid weight, so something should be higher*/
				high_fringe = i;
				high_val = weight[i];
			}
		}
//printf("hf:%u\n", high_fringe);
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
	time = clock() - time;
	double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
	printf("Exec time: %f seconds\t", time_taken);

	//Output
	printf("MBW: %u\n", weight[t]);
	
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
	
	free(status);
	free(weight);
	free(parent);
	free(res);
	return;
}


void mbp_ewd_heap(wul_graph* g, uint32_t s, uint32_t t) {
	if (s > g->size || t > g->size) {
		fprintf(stderr, "Source(%u) or dest(%u) out of range(%lu)",
				s, t, g->size);
		exit(EXIT_FAILURE);
	}
	if(s == t) {
		fprintf(stderr, "Source and dest are the same.");
	}
	/*
	 * u = unseen
	 * f = fringe
	 * i = in tree
	 */
	uint8_t*  status = (uint8_t* ) s_malloc(sizeof(uint8_t)  * g->size);
	uint32_t* weight = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);
	uint32_t* parent = (uint32_t*) s_malloc(sizeof(uint32_t) * g->size);

	clock_t time;
	time = clock();

	for (uint32_t i = 0; i < g->size; i++) {
		status[i] = 'u';
	}
	status[s] = 'i';

	heap* h = gen_heap(INIT_HEAP_SIZE);
	edge insert;

	w_node* curr = (w_node*) g->vertices[s];
	while (curr != NULL) {
		status[curr->neighbor] = 'f';
		insert.src = s;
		insert.dst = curr->neighbor;
		insert.wt  = curr->weight;
		heap_insert(h, insert);
		weight[curr->neighbor] = curr->weight;
		parent[curr->neighbor] = s;
		curr = (w_node*) curr->next;
	}
	edge high_fringe;
	while(h->size != 0) {
		high_fringe = heap_max(h);
		heap_del(h, 1);
//printf("hf:(s:%u-t:%u-w:%u)\n", high_fringe.src, high_fringe.dst, high_fringe.wt);
		status[high_fringe.dst] = 'i';//TODO dst?
		curr = (w_node*) g->vertices[high_fringe.dst];
		while(curr != NULL ) {
			if(status[curr->neighbor] == 'u'){
				status[curr->neighbor] = 'f';
				insert.src = high_fringe.dst;
				insert.dst = curr->neighbor;
				insert.wt  = curr->weight;
				heap_insert(h, insert);
				parent[curr->neighbor] = high_fringe.dst;
				weight[curr->neighbor] = min(weight[high_fringe.dst], curr->weight);
			}
			else if(status[curr->neighbor] == 'f' &&
					weight[curr->neighbor] <
					min(weight[high_fringe.dst], curr->weight)){
				parent[curr->neighbor] = high_fringe.dst;
				weight[curr->neighbor] = min(weight[high_fringe.dst], curr->weight);
			}
			curr = (w_node*) curr->next;
		}
	}
	time = clock() - time;
	double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
	printf("Exec time: %f seconds\t", time_taken);

	//Output
	printf("MBW: %u\n", weight[t]);
	
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
	
	free(status);
	free(weight);
	free(parent);
	free(res);
	return;
}


inline uint32_t min(uint32_t x, uint32_t y) {
	if (x < y) {
		return x;
	}
	return y;
}


void mbp_kruskal(wul_graph* g, uint32_t s, uint32_t t) {
	heap* ke = gen_heap(INIT_HEAP_SIZE);
	uint32_t* parent = (uint32_t*) s_malloc(INIT_HEAP_SIZE * sizeof(uint32_t));
	uint32_t* rank   = (uint32_t*) s_malloc(INIT_HEAP_SIZE * sizeof(uint32_t));
	for(uint32_t i=0; i < INIT_HEAP_SIZE; i++) {
		parent[i] = 0;
		rank[i] = 0;
	}//null out the parent array

	edge current;
	w_node* curr;
	uint32_t r1, r2;

	clock_t time;
	time = clock();

	for(uint32_t i=0; i < g->size; i++) {
		curr = (w_node*) g->vertices[i];
		while (curr != NULL) {
			if (i < curr->neighbor) {
				current.src = i;
				current.dst = curr->neighbor;
				current.wt  = curr->weight;
				heap_insert(ke, current);
			}
			curr = (w_node*) curr->next;
		}
	}//sort in non-increasing order
	wul_graph result;
	result.size = SIZE;
	result.vertices = (w_node**) s_malloc(sizeof(w_node*) * result.size);
	for (uint32_t i = 0; i < result.size; i++) {
		result.vertices[i] = NULL;
	}//ensure all neighbor lists are initially NULL

/*
	while(ke->size != 0) {
		current = heap_max(ke);
		heap_del(ke, 1);
		printf("(s:%u-t:%u-w:%u)\n",current.src, current.dst, current.wt);
	}
*/

	while(ke->size != 0) {
		current = heap_max(ke);
		heap_del(ke, 1);
		r1 = find(parent, current.src);
		r2 = find(parent, current.dst);
		if(r1 != r2) {
			krusk_union(rank, parent, r1, r2);
			add_neighbor(&result, current.src, current.dst, current.wt);
		}
	}
//	vis_wul_graph(tm, &result);
	uint32_t max_bandwidth;
	uint8_t*  status = (uint8_t* ) s_malloc(sizeof(uint8_t)  * result.size);
	for (uint32_t i=0; i < result.size; i++) {
		status[i] = 'u';
	}
//	status[s] = 'd';
	uint32_t* stack = (uint32_t*) s_malloc(SIZE * sizeof(uint32_t));
	uint32_t s_pos = 0;
	uint32_t curr_v;

/*
 *Extracting the MBP and MB is miserable
 *The tree has been verified as correct by visually inspecting the generated graph
 */




	//DFS
	stack[s_pos++] = s;
	while (s_pos) {
		curr_v = stack[--s_pos];
		if(curr_v == t) {
			while(s_pos) {
				printf("%u..", stack[--s_pos]);
				//TODO somehow track BW
			}
		}
		else if (status[curr_v] != 'd') {
			status[curr_v] = 'd';
			curr = (w_node*) result.vertices[curr_v];
			while (curr != NULL) {
				stack[s_pos++] = curr->neighbor;
				curr = (w_node*) curr->next;
			}
		}
//		for (uint32_t i=0; i < s_pos; i++) {
//			printf("%u..",stack[i]);
//		}
//		printf("%u\n", s_pos);
	}


	time = clock() - time;
	double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
	printf("Exec time: %f seconds\n", time_taken);


	//Output
/*
	printf("MBW: %u\n", weight[t]);
	uint32_t* res = (uint32_t*) malloc(g->size * sizeof(uint32_t));
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
	free(parent);
	free(rank);
	return;
}


uint32_t find(uint32_t* parent, uint32_t val) {
	uint32_t w = val;
	while(parent[w] != 0) {
		w = parent[w];
	}
	return w;
}


void krusk_union(uint32_t* rank, uint32_t* parent, uint32_t r1, uint32_t r2) {
	if     (rank[r1] > rank[r2]) {
		parent[r2] = r1;
	}
	else if(rank[r1] < rank[r2]) {
		parent[r1] = r2;
	}
	else {
		parent[r1] = r2;
		rank[r2]++;
	}
	return;
}


void* s_malloc(size_t size) {
	void* ptr;
	if((ptr = malloc(size))) {
		return ptr;
	}
	else {
		fprintf(stderr, "ERROR Failed to malloc\n");
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
