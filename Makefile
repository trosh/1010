CC := gcc

.PHONY: run

1010: main.c tt.c bk.c
	$(CC) -o $@ $< -lncurses

run: 1010
	./1010

clean: 1010
	rm ./1010
