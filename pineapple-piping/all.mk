.PHONY: all

all: gen sol

gen: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(STANDARD_FLAGS) -o gen gen.c pinepipe.c

sol: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(STANDARD_FLAGS) -o sol sol.c pinepipe.c
