CC = gcc
C_OPTS = -Wall -Wextra -pedantic -DEBUG -O0 -g
LINK_OPTS =
sources = $(wildcard *.c)
headers = $(wildcard *.h)
objects = $(patsubst %.c,%.o,$(sources))

automata: $(objects)
	$(CC) $(C_OPTS) -o $@ $^

%.o: %.c $(headers)
	$(CC) $(C_OPTS) -g -c $(LINK_OPTS) $< -o $@


clean:
	rm -f $(objects) automata
.PHONY: clean
