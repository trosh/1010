#include <ncurses.h>

int main() {
	char c;
	c = 0;
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	while (1) {
		clear();
		mvprintw(1, 1, "%d", c);
		c = getch();
	}
	return 0;
}
