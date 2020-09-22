CC = gcc
C_OPTS = -Wall -Wextra -pedantic -DEBUG
LINK_OPTS =
sources = $(wildcard *.c)
headers = $(wildcard *.h)
objects = $(patsubst %.c,%.o,$(sources))


%.o: %.c $(headers)
	$(CC) $(C_OPTS) -g -c $(LINK_OPTS) $< -o $@

clean:
	rm -f $(objects)
.PHONY: clean
