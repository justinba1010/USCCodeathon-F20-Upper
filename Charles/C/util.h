#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#define MustMalloc(size) \
        __extension__ ({ \
                void* _res = malloc(size); \
                if (_res == NULL) { err(1, "malloc failed"); } \
                _res;})

/* "raw" debug printf */
#ifdef EBUG
#define dbrprintf(...) do { fprintf(stderr, __VA_ARGS__); } while (0)
#else
#define dbrprintf(...)
#endif

/* debug printf */
#ifdef EBUG
#define dbprintf(...) do { fprintf(stderr, "%s:%d:%s(): ",__FILE__, __LINE__, __func__);\
             fprintf(stderr, __VA_ARGS__); } while (0)
#else
#define dbprintf(...)
#endif

#ifdef EBUG
#define dbprint_node(node) do { nocsim_print_node(stderr, node); } while(0)
#else
#define dbprint_node(node)
#endif

#define UNUSED(x) (void)(x)

#define StringEquals(s1, s2) (strcmp(s1, s2) == 0)

#define StringAppend(_base, _append) \
	__extension__ ({ \
		char* _append_result; \
		do { \
			size_t _baselen = strlen(_base); \
			size_t _appendlen = strlen(_append); \
			_append_result = MustMalloc(_baselen+_appendlen+1); \
			_append_result[0] = 0; \
			strcat(_append_result, _base); \
			strcat(_append_result, _append); \
		while(0) \
		_append_result; })

#endif /* UTIL_H */
