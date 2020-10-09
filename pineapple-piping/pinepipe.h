#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define W_MAX 1000
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

//graphing
wul_graph* gen_wul_graph         (size_t     s, uint32_t    d);
void       vis_wul_graph         (wul_graph* g, const char* s);
void       destroy_wul_graph     (wul_graph* g               );
void       serialize_wul_graph   (wul_graph  g               );
wul_graph  deserialize_wul_graph (size_t     s               );

//helper
uint32_t min          (uint32_t  , uint32_t                    );
void     shuffle      (uint32_t* , size_t                      );
void     add_neighbor (wul_graph*, uint32_t, uint32_t, uint32_t);
bool     is_connected (wul_graph*, uint32_t, uint32_t          );

//routing
uint32_t mbp_ewd (wul_graph* g, uint32_t s, uint32_t t);

//safe standard memory management
void* s_malloc  (size_t        );
void* s_calloc  (size_t        );
void* s_realloc (void* , size_t);

