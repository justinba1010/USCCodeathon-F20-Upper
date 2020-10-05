GVIZ=$(shell ls *.dot 2>/dev/null | while read -r f ; do echo "$$(basename $$f .dot).png" ; done)
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99

.PHONY: all dot png

all: gen sol

gen: gen.c gen.h
	$(CC) $(CFLAGS) -O3 -o gen gen.c

sol: sol.c sol.h
	$(CC) $(CFLAGS) -O3 -o gen gen.c

#debug
d_gen: gen.c gen.h
	$(CC) $(CFLAGS) -g3 -o gen gen.c

d_sol: sol.c sol.h
	$(CC) $(CFLAGS) -g3 -o gen gen.c

#visualization
dot: $(GVIZ)

%.png: %.dot
	dot -Tpng $^ > $@

clean:
	rm -f *.dot *.png gen sol
