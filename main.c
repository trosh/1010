#include <stdio.h>   /* printf, putchar, fputs, puts */
#include <stdlib.h>  /* calloc, srand, rand */
#include <time.h>    /* time */
#include <ncurses.h> /* ... */

/********** TT ***********/

int *tt; /* ROW MAJOR 10x10 BITARRAY IN INT[4] */
int xs[10], ys[10]; /* ROWS AND COLS TO POP */

/* READ TT
 *     i>>5 <=> i/32
 *     i&31 <=> i%32
 */
inline int rtt(int x, int y) {
	int i = 10*y+x; /* 2D -> 1D */
	return tt[i>>5] & (1<<(i&31));
}

/* WRITE TRUE TT */
inline void wttt(int x, int y) {
	int i = 10*y+x;
	tt[i>>5] |= (1<<(i&31));
}

/* WRITE FALSE TT */
inline void wftt(int x, int y) {
	int i = 10*y+x;
	tt[i>>5] &= ~(1<<(i&31));
}

/* PRINT TT */
void printtt() {
	int x, y;
	for (y=0; y<10; y++) {
		//printf("%d ", y);
		for (x=0; x<10; x++)
			if (rtt(x, y)) {
				attron(COLOR_PAIR(3));
				addstr("  ");
				attroff(COLOR_PAIR(3));
			} else
				addstr("  ");
		attron(COLOR_PAIR(1));
		addstr("  ");
		attroff(COLOR_PAIR(1));
		addch('\n');
	}
	attron(COLOR_PAIR(1));
	addstr("                      ");
	attroff(COLOR_PAIR(1));
}

void updatett() {
	int x, y, f, xc, yc, i;
	xc = 0;
	yc = 0;
	for (x=0; x<10; x++) { /* FIND FULL COLS */
		f = 1;
		for (y=0; y<10; y++)
			if (!rtt(x, y)) {
				f = 0;
				break;
			}
		if (f) {
			xs[xc] = x;
			xc++;
		}
	}
	for (y=0; y<10; y++) { /* FIND FULL ROWS */
		f = 1;
		for (x=0; x<10; x++)
			if (!rtt(x, y)) {
				f = 0;
				break;
			}
		if (f) {
			ys[yc] = y;
			yc++;
		}
	}
	for (i=0; i<xc; i++) /* EMPTY FULL COLS */
		for (y=0; y<10; y++)
			wftt(xs[i], y);
	for (i=0; i<yc; i++) /* EMPTY FULL ROWS */
		for (x=0; x<10; x++)
			wftt(x, ys[i]);
}

/********** BK ***********/

#define NBK 19

/* BLOCKS IN ROW MAJOR 5x5 BITARRAYS IN INT */
int bk[NBK] = {1,3,7,15,31,33,
               35,67,97,98,99,
               1057,1063,4231,
               7201,7300,7399,
               33825,1082401};

/* READ BK[N] */
inline int rbk(int n, int x, int y) {
	int i = 5*y+x;
	return bk[n] & (1<<i);
}

/* PRINT BK[N] AT POS X, Y */
void printbkpos(int n, int x, int y) {
	int i, j;
	attron(COLOR_PAIR(2));
	for (j=0; j<5; j++)
		for (i=0; i<5; i++)
			if (rbk(n, i, j))
				mvaddstr(y+j, x+i<<1, "  ");
	attroff(COLOR_PAIR(2));
}

/* DOES BLOCK FIT IN TT AT POS X, Y ? */
int bkfits(int n, int x, int y) {
	int i, j;
	char f = 0;
	for (i=0; i<5; i++)
		for (j=0; j<5; j++) {
			if (!rbk(n, i, j))
				continue;
			if (x+i>9 || y+j>9)
				return -1; /* OUT OF BOUNDS */
			if (rtt(x+i, y+j))
				f = 1; /* DOES NOT FIT */
		}
	if (f) return 0;
	return 1; /* FITS */
}

void bkinsert(int n, int x, int y) {
	int i, j;
	for (i=0; i<5; i++)
		for (j=0; j<5; j++)
			if (rbk(n, i, j))
				wttt(x+i, y+j);
}

/********** MAIN *********/

int main(int argc, char **argv) {
	int bknum, x, y;
	char f, c;
	tt = (int*)calloc(4, 4); /* !!! HARDCODED SIZEOF(INT) = 4 */
	srand(time(NULL));
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	if (has_colors() == FALSE) {
		endwin();
		puts("no color");
		return 1;
	}
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_WHITE, COLOR_WHITE);
	while (1) {
		bknum = rand()%NBK; // PSEUDO-RANDOM !
		f = x = y = 0;
		while (1) {
			clear();
			printtt();
			printbkpos(bknum, x, y);
			refresh();
			c = getch();
			mvprintw(12, 1, "%d", c);
			switch (c) {
			case 4 : {
				if (x>0
				 && bkfits(bknum, x-1, y)!=-1)
					x--;
				break; }
			case 2 : {
				if (y<9
				 && bkfits(bknum, x, y+1)!=-1)
					y++;
				break; }
			case 3 : {
				if (y>0
				 && bkfits(bknum, x, y-1)!=-1)
					y--;
				break; }
			case 5 : {
				if (x<9
				 && bkfits(bknum, x+1, y)!=-1)
					x++;
				break; }
			case 10 :
			case 32 : {
				if (bkfits(bknum, x, y)==1)
					f = 1;
				break; }
			case 'q' : {
				endwin();
				return 0; }
			}
			if (f) break;
		}
		bkinsert(bknum, x, y);
		updatett();
	}
	endwin();
	return 0;
}
