#include <assert.h>
#include "pinepipe.h"

int main(int argc, char** argv) {
	//get first line
	size_t size, s, t;
	scanf("%zu%zu%zu\n", &size, &s, &t);
	wul_graph g = deserialize_wul_graph(size);
	printf("%u\n", mbp_ewd(&g, s, t));
	if (argc > 1) {
		vis_wul_graph(&g, argv[1]);
	}
	destroy_wul_graph(&g);
	return EXIT_SUCCESS;
}
