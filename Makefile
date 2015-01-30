CC := gcc

.PHONY: run

1010: main.c
	$(CC) -o $@ $< -lncurses

run: 1010
	./1010
