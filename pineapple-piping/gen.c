#include "pinepipe.h"

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

	if (argc == 6) {
		vis_wul_graph(g, argv[5]);
	}
	destroy_wul_graph(g);
	free(g);
	return EXIT_SUCCESS;
}
