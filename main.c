/***********************/
/*** 1010 COPY       ***/
/*** BY TROSH        ***/
/*** FUN TIMES IN C! ***/
/*** ORIGINAL GAME   ***/
/*** BY GRAM GAMES   ***/
/***********************/

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
	move(0, 0);
	for (y=0; y<10; y++) {
		for (x=0; x<10; x++)
			if (rtt(x, y)) {
				attron(COLOR_PAIR(2));
				addstr("  ");
				attroff(COLOR_PAIR(2));
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
void printbk(char n, char x, char y, char c) {
	char i, j;
	attron(COLOR_PAIR(c));
	for (j=0; j<5; j++)
		for (i=0; i<5; i++)
			if (rbk(n, i, j))
				if (rtt(x+i, y+j))
					mvaddstr(y+j, x+i<<1, "xx");
				else
					mvaddstr(y+j, x+i<<1, "  ");
	attroff(COLOR_PAIR(c));
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

/**************************/
/*** TODO:              ***/
/*** AI                 ***/
/*** SCORE              ***/
/**************************/

int main(int argc, char **argv) {
	char f;      /* FLAG */
	char c;      /* INPUT CHARACTER */
	char bks[3]; /* BLOCKS GIVEN TO USER */
	char bka[3]; /* FLAGS OF BLOCKS AVAILABLE ; TODO -> BITARRAY */
	char b;      /* INDEX OF USER CHOSEN BLOCK */
	char sc;     /* SUB COUNTER */
	char x, y;   /* POSITION OF BLOCK */
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
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_BLUE);
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);
	sc = 0;
	while (1) {
		if (sc == 0) {
			bks[0] = rand()%NBK; /* PSEUDO-RANDOM ! */
			bks[1] = rand()%NBK;
			bks[2] = rand()%NBK;
			bka[0] = bka[1] = bka[2] = 1;
			b = 0;
			                sc = 1;
		} else if (sc == 2) sc = 0;
		else                sc = 2;
		f = 0;
		for (c=0; c<3; c++) {
			if (!bka[c]) continue;
			for (y=0; y<5; y++) {
				for (x=0; x<5; x++) {
					if (bkfits(bks[c], x, y) == 1) {
						f = 1;
						break; /* FITS */
					}
				}
				if (f) break;
			}
			if (f) break;
		}
		if (!f) {
			attron(COLOR_PAIR(3));
			mvaddstr(3, 4, "            ");
			mvaddstr(4, 4, "  YOU LOSE  ");
			mvaddstr(5, 4, "            ");
			attroff(COLOR_PAIR(3));
			getch();
			break;
		}
		f = 0;
		x = y = 5;
		while (1) {
			clear();
			printtt();
			mvaddch(11,  1, '1');
			mvaddch(11, 12, '2');
			mvaddch(11, 24, '3');
			if (bka[0]) printbk(bks[0],  0, 12, 3);
			if (bka[1]) printbk(bks[1],  5, 12, 4);
			if (bka[2]) printbk(bks[2], 10, 12, 5);
			printbk(bks[b], x, y, b+3);
			refresh();
			c = getch();
			switch (c) {
			case 49 : { /* 1 */
				if (!bka[0]) break;
				b = 0;
				x = y = 5;
				break; }
			case 50 : { /* 2 */
				if (!bka[1]) break;
				b = 1;
				x = y = 5;
				break; }
			case 51 : { /* 3 */
				if (!bka[2]) break;
				b = 2;
				x = y = 5;
				break; }
			case 4 : { /* LEFT */
				if (x>0
				 && bkfits(bks[b], x-1, y)!=-1)
					x--;
				break; }
			case 2 : { /* DOWN */
				if (y<9
				 && bkfits(bks[b], x, y+1)!=-1)
					y++;
				break; }
			case 3 : { /* RIGHT */
				if (y>0
				 && bkfits(bks[b], x, y-1)!=-1)
					y--;
				break; }
			case 5 : { /* UP */
				if (x<9
				 && bkfits(bks[b], x+1, y)!=-1)
					x++;
				break; }
			case 10 :   /* ENTER */
			case 32 : { /* SPACE */
				if (bkfits(bks[b], x, y)==1)
					f = 1;
				break; }
			case 'q' : { /* Q */
				endwin();
				return 0; }
			}
			if (f) break;
		}
		bkinsert(bks[b], x, y);
		bka[b] = 0; /* BLOCK B NOT AVAILABLE */
		for (c=0; c<3; c++) { /* CHOOSE NEW B */
			if (b==c /* DON'T CHECK BKA[B] */
			 || !bka[c]) continue;
			b = c;
			break;
		}
		updatett();
		if (!(bka[0]+bka[1]+bka[2])) continue;
	}
	endwin();
	return 0;
}
