/********** BK ***********/
/* Blocks for 1010       */
/*************************/

#define NBK 19

/* Blocks in row major 5x5 bitarrays in int */
int bk[NBK] = {1,3,7,15,31,33,
               35,67,97,98,99,
               1057,1063,4231,
               7201,7300,7399,
               33825,1082401};

/* Number of squares in each block (required for scoring) */
int bksquares[NBK] = {1,2,3,4,5,
                      2,3,3,3,3,
                      4,3,5,5,5,
                      5,9,4,5};

/* Read bk[n] */
int rbk(int n, int x, int y) {
    int i = 5*y+x;
    return bk[n] & (1<<i);
}

/* Print bk[n] at pos x, y */
void printbk(char n, char x, char y, char c) {
    char i, j;
    attron(COLOR_PAIR(c));
    for (j=0; j<5; j++)
    for (i=0; i<5; i++) {
        if (rbk(n, i, j)) {
            attron(A_UNDERLINE);
            if (x < 10 && y < 10 /* Block on the game board */
             && rtt(x+i, y+j)) { /* And over existing tile */
                mvaddstr((y+j), (x+i)*3, "xx");
            } else { /* Hint board or valid tile */
                mvaddstr((y+j), (x+i)*3, "  ");
            }
            /* Print borders between tiles (rightwards) */
            mvadd_wch((y+j), (x+i)*3+2, &vert_sep_right);
            attroff(A_UNDERLINE);
            // The downwards border is always present (with A_UNDERLINE)
        }
    }
    attroff(COLOR_PAIR(c));
}

/* Does block n fit in tt at pos x, y ? */
int bkfits(int n, int x, int y) {
    int i, j;
    for (i=0; i<5; i++) /* For each sub block of bk */
    for (j=0; j<5; j++) {
        if (!rbk(n, i, j)) /* Is it an actual sub block ? */
            continue;
        if (x+i > 9
         || y+j > 9)
            return -1; /* Out of bounds */
    }
    for (i=0; i<5; i++) /* For each sub block of bk */
    for (j=0; j<5; j++) {
        if (!rbk(n, i, j)) /* Is it an actual sub block ? */
            continue;
        if (rtt(x+i, y+j))
            return 0; /* Does not fit */
    }
    return 1; /* Fits */
}

void bkinsert(int n, int x, int y) {
    int i, j;
    for (i=0; i<5; i++)
    for (j=0; j<5; j++)
        if (rbk(n, i, j))
            wttt(x+i, y+j);
}
