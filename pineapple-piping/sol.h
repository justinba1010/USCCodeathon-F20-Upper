#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "u_rand.h"


#define SIZE 6
#define W_MAX 100
#define VIS_SIZE_MAX 10
#define G_ONE_DEG 3
#define G_TWO_NUMER 1
#define G_TWO_DENOM 5
#define INIT_HEAP_SIZE 1000
#define RUNS 5

/*
options:
{(w)eighted, (u)nweighted} * {(d)irected, (u)ndirected}, *
{(m)atrix, (l)inked list}

 *nodes:
	u,w
	matrix methods do not need nodes

graphs:
	wdm
	wdl
	wum
	wul
	udm
	udl
	uum
	uul
 */

typedef struct {
	uint32_t neighbor;
	uint32_t weight;
	struct w_node* next;
} w_node;//weighted node

typedef struct {
	size_t size;
	struct w_node** vertices;
} wul_graph;//weighted, undirected, linked list

typedef struct {
	uint32_t src;
	uint32_t dst;
	uint32_t wt;
} edge;

typedef struct {
	size_t size;
	size_t cap;
	edge* H;
} heap;

struct tm *tm;//for visulaization, global for kruskal tree


//TODO disambiguate heap and max heap

//graphing
wul_graph* gen_graph_one    (size_t, uint32_t                        );
wul_graph* gen_graph_two    (size_t, uint32_t, uint32_t              );
uint32_t   get_connect_prob (uint32_t, uint32_t                      );
void       vis_wul_graph    (struct tm*, wul_graph*                  );
void       shuffle          (uint32_t*, size_t                       );
void       add_neighbor     (wul_graph*, uint32_t, uint32_t, uint32_t);
bool       is_connected     (wul_graph*, uint32_t, uint32_t          );
void       destroy_wul_graph(wul_graph*                              );

//heap
heap*    gen_heap    (size_t         );
edge     heap_max    (heap*          );
void     heap_insert (heap*, edge    );
void     heap_del    (heap*, uint32_t);
void     vis_heap    (heap*          );
void     destroy_heap(heap*          );

//routing
void mbp_ewd     (wul_graph* g, uint32_t s, uint32_t t);
void mbp_ewd_heap(wul_graph* g, uint32_t s, uint32_t t);
void mbp_kruskal (wul_graph* g, uint32_t s, uint32_t t);

//helper
uint32_t min        (uint32_t, uint32_t                      );
uint32_t find       (uint32_t*, uint32_t                     );
void     krusk_union(uint32_t*, uint32_t*, uint32_t, uint32_t);

void* s_malloc(size_t);
void* s_calloc(size_t);
void* s_realloc(void*, size_t);

