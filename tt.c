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
