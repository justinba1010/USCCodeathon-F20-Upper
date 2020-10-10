GVIZ=$(shell ls *.dot 2>/dev/null | while read -r f ; do echo "$$(basename $$f .dot).svg" ; done)
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99
RELEASE_FLAGS=-O3 -march=native
DEBUG_FALGS=-g3
STANDARD_FLAGS=-Og

.PHONY: release debug all dot svg

release: r_gen r_sol

debug: d_gen d_sol

all: gen sol

#release
r_gen: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o gen gen.c pinepipe.c

r_sol: sol.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o sol sol.c pinepipe.c

#debug
d_gen: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o gen gen.c pinepipe.c

d_sol: sol.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o sol sol.c pinepipe.c

#all
gen: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(STANDARD_FLAGS) -o gen gen.c pinepipe.c

sol: sol.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(STANDARD_FLAGS) -o sol sol.c pinepipe.c

#visualization
dot: $(GVIZ)

%.svg: %.dot
	dot -Tsvg $^ > $@

clean:
	rm -f *.dot *.png *.svg gen sol
