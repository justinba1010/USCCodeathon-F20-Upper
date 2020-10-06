#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define W_MAX 100
#define VIS_SIZE_MAX 10

/*
Naming options:
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


typedef struct w_node{
	uint32_t neighbor;
	uint32_t weight;
	struct w_node* next;
} w_node;//weighted node

typedef struct wul_graph{
	size_t size;
	w_node** vertices;
} wul_graph;//weighted, undirected, linked list

//graphing
wul_graph* gen_wul_graph       (size_t s, uint32_t d                    );
void       vis_wul_graph       (const char* s, wul_graph*               );
void       shuffle             (uint32_t*, size_t                       );
void       add_neighbor        (wul_graph*, uint32_t, uint32_t, uint32_t);
bool       is_connected        (wul_graph*, uint32_t, uint32_t          );
void       serialize_wul_graph (wul_graph g                             );
void       destroy_wul_graph   (wul_graph* g                            );

//safe standard memory management
void* s_malloc(size_t);
void* s_calloc(size_t);
void* s_realloc(void*, size_t);

