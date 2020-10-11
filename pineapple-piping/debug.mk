.PHONY: debug

debug: gen sol

gen: gen.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o gen gen.c pinepipe.c

sol: sol.c pinepipe.c pinepipe.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o sol sol.c pinepipe.c
