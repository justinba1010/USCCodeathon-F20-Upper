GVIZ=$(shell ls *.dot 2>/dev/null | while read -r f ; do echo "$$(basename $$f .dot).svg" ; done)
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99
RELEASE_FLAGS=-O3 -march=native
DEBUG_FALGS=-g3
STANDARD_FLAGS=-Og

.PHONY: release debug all dot svg

release debug all: gen.c pinepipe.c pinepipe.h
	make -f $@.mk CC="$(CC)" CFLAGS="$(CFLAGS)" \
	RELEASE_FLAGS="$(RELEASE_FLAGS)" DEBUG_FLAGS="$(DEBUG_FLAGS)" \
	STANDARD_FLAGS="$(STANDARD_FLAGS)"

#visualization
dot: $(GVIZ)

%.svg: %.dot
	dot -Tsvg $^ > $@

clean:
	rm -f *.dot *.svg gen sol
