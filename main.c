#include <stdio.h>  /* printf, putchar, fputs */
#include <stdlib.h> /* calloc, srand, rand */
#include <time.h>   /* time */

/* DEBUGGING
void printbin(int x) {
	while (x) {
		if (x&1)
			putchar('1');
		else
			putchar('0');
		x>>=1;
	}
	putchar('\n');
}
*/

/* CLEAR SCREEN */
void cls() {
	int i, j;
	fputs("\033[1;1H", stdout);
	for (i=0; i<10; i++)
		puts("                    ");
	fputs("\033[1;1H", stdout);
}

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
			if (rtt(x, y))
				fputs("\033[47m  \033[0m", stdout);
			else
				fputs("  ", stdout);
		puts("\033[42m  \033[0m");
	}
	puts("\033[42m                      \033[0m");
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

/* WRITE TRUE BK[N]
inline void wtbk(int n, int x, int y) {
	int i = 5*y+x;
	bk[n] |= (1<<i);
}
*/

/* WRITE FALSE BK[N]
inline void wfbk(int n, int x, int y) {
	int i = 5*y+x;
	bk[n] &= ~(1<<i);
}
*/

/* PRINT BK[N] */
void printbk(int n) {
	int x, y;
	for (y=0; y<5; y++) {
		for (x=0; x<5; x++)
			if (rbk(n, x, y))
				fputs("\033[47m  \033[0m", stdout);
			else
				fputs("  ", stdout);
		putchar('\n');
	}
}

/* PRINT BK[N] AT POS X, Y */
void printbkpos(int n, int x, int y) {
	int i, j;
	fputs("\033[41m", stdout);
	for (j=0; j<5; j++)
		for (i=0; i<5; i++)
			if (rbk(n, i, j))
				printf("\033[%d;%dH  ", y+j+1, (x+i)*2+1);
	fputs("\033[0m", stdout);
}

/* DOES BLOCK FIT IN TT AT POS X, Y ? */
int bkfits(int n, int x, int y) {
	int i, j;
	for (i=0; i<5; i++)
		for (j=0; j<5; j++) {
			if (!rbk(n, i, j))
				continue;
			if (x+i>9 || y+j>9)
				return -1; /* OUT OF BOUNDS */
			if (rtt(x+i, y+j))
				return 0;  /* DOES NOT FIT */
		}
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
	while (1) {
		//printtt();
		bknum = rand()%NBK; // PSEUDO-RANDOM !
		//printbk(bknum);
		f = x = y = 0;
		while (1) {
			cls();
			printtt();
			printbkpos(bknum, x, y);
			fputs("\033[12;1H", stdout);
			scanf("%c", &c);
			switch (c) {
			case 'h' : {
				if (x>0
				 && bkfits(bknum, x-1, y)!=-1)
					x--;
				break; }
			case 'j' : {
				if (y<9
				 && bkfits(bknum, x, y+1)!=-1)
					y++;
				break; }
			case 'k' : {
				if (y>0
				 && bkfits(bknum, x, y-1)!=-1)
					y--;
				break; }
			case 'l' : {
				if (x<9
				 && bkfits(bknum, x+1, y)!=-1)
					x++;
				break; }
			case 'g' : {
				if (bkfits(bknum, x, y)==1)
					f = 1;
				break; }
			}
			if (f) break;
		}
		/*
		while (1) {
			fputs("x y ? ", stdout);
			scanf("%d %d", &x, &y);
			if (bkfits(bknum, x, y)!=1)
				continue;
			break;
		}
		*/
		bkinsert(bknum, x, y);
		updatett();
	}
	return 0;
}
