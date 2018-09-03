CC := gcc

.PHONY: run

1010: main.c tt.c bk.c
	$(CC) -o $@ $< -lncursesw -D_XOPEN_SOURCE_EXTENDED

run: 1010
	./1010

clean: 1010
	rm -f ./1010
