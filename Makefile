CC := gcc

.PHONY: run

1010: main.c
	$(CC) -o $@ $<

run: 1010
	./1010
