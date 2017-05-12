/********** BK ***********/

#define NBK 19

/* BLOCKS IN ROW MAJOR 5x5 BITARRAYS IN INT */
int bk[NBK] = {1,3,7,15,31,33,
               35,67,97,98,99,
               1057,1063,4231,
               7201,7300,7399,
               33825,1082401};

/* NUMBER OF SQUARES IN EACH BLOCK (REQUIRED FOR SCORING) */
int bksquares[NBK] = {1,2,3,4,5,
                      2,3,3,3,3,
                      4,3,5,5,5,
                      5,9,4,5};

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
    for (i=0; i<5; i++) {
        if (rbk(n, i, j)) {
            if (x < 10 && y < 10 /* BLOCK ON THE GAME BOARD */
             && rtt(x+i, y+j)) { /* AND OVER EXISTING TILE */
                mvaddstr((y+j)*2, (x+i)*3, "xx");
            } else { /* HINT BOARD OR VALID TILE */
                mvaddstr((y+j)*2, (x+i)*3, "  ");
            }
            /* PRINT BORDERS BETWEEN TILES (RIGHTWARDS) */
            if (i < 4
             && rbk(n, i+1, j)) {
                mvaddch((y+j)*2, (x+i)*3+2, ACS_VLINE);
            }
            /* PRINT BORDERS BETWEEN TILES (DOWNWARDS) */
            if (j < 4
             && rbk(n, i, j+1)) {
                mvaddch((y+j)*2+1, (x+i)*3,   ACS_HLINE);
                mvaddch((y+j)*2+1, (x+i)*3+1, ACS_HLINE);
            }
            /* PRINT BORDERS BETWEEN TILES (DOWN-RIGHTWARDS) */
            if (i < 4 && j < 4
             && rbk(n, i+1, j) && rbk(n, i, j+1)
             && rbk(n, i+1, j+1)) {
                mvaddch((y+j)*2+1, (x+i)*3+2, ACS_PLUS);
            }
        }
    }
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
    }
    for (i=0; i<5; i++) /* FOR EACH SUB BLOCK OF BK */
    for (j=0; j<5; j++) {
        if (!rbk(n, i, j)) /* IS IT AN ACTUAL SUB BLOCK ? */
            continue;
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
