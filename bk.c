/********** BK ***********/

#define NBK 19

/* BLOCKS IN ROW MAJOR 5x5 BITARRAYS IN INT */
int bk[NBK] = {1,3,7,15,31,33,
               35,67,97,98,99,
               1057,1063,4231,
               7201,7300,7399,
               33825,1082401};

//char bkprobs[] = {0,0,0,0,

/* READ BK[N] */
int rbk(int n, int x, int y) {
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

/* DOES BLOCK N FIT IN TT AT POS X, Y ? */
int bkfits(int n, int x, int y) {
    int i, j;
    for (i=0; i<5; i++) /* FOR EACH SUB BLOCK OF BK */
    for (j=0; j<5; j++) {
        if (!rbk(n, i, j)) /* IS IT AN ACTUAL SUB BLOCK ? */
            continue;
        if (x+i > 9
         || y+j > 9)
            return -1; /* OUT OF BOUNDS */
        if (rtt(x+i, y+j))
            return 0; /* DOES NOT FIT */
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

