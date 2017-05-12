/********** TT ************/
/* Grid of tiles for 1010 */
/**************************/

int *tt; /* row major 10x10 bitarray in int[4] */
/* Rows and cols to pop */
int xs[10];
int ys[10];

/* Read tt
 *     i>>5 <=> i/32
 *     i&31 <=> i%32
 */
int rtt(int x, int y) {
    int i = 10*y+x; /* 2D -> 1D */
    return tt[i>>5] & (1<<(i&31));
}

/* Write true tt */
void wttt(int x, int y) {
    int i = 10*y+x;
    tt[i>>5] |= (1<<(i&31));
}

/* Write false tt */
void wftt(int x, int y) {
    int i = 10*y+x;
    tt[i>>5] &= ~(1<<(i&31));
}

/* Print tt */
void printtt() {
    int x, y;
    move(0, 0);
    for (y=0; y<10; y++) {
        for (x=0; x<10; x++) {
            if (rtt(x, y)) {
                attron(COLOR_PAIR(2));
                addstr("  ");
                attroff(COLOR_PAIR(2));
                /* Print borders between tiles (rightwards) */
                if (x < 9 && rtt(x+1, y)) {
                    attron(COLOR_PAIR(2));
                }
            } else
                addstr("  ");
            if (x < 9) {
                addch(ACS_VLINE);
            }
            attroff(COLOR_PAIR(2));
        }
        attron(COLOR_PAIR(1));
        addstr("  ");
        attroff(COLOR_PAIR(1));
        addch('\n');
        if (y < 9) {
            for (x=0; x<10; x++) {
                /* Borders between tiles (downwards) */
                if (y < 9
                 && rtt(x, y) && rtt(x, y+1)) {
                    attron(COLOR_PAIR(2));
                }
                addch(ACS_HLINE);
                addch(ACS_HLINE);
                attroff(COLOR_PAIR(2));
                /* Borders between tiles (down-rightwards) */
                if (x < 9) {
                    if (y < 9
                     && rtt(x, y)   && rtt(x+1, y)
                     && rtt(x, y+1) && rtt(x+1, y+1)) {
                        attron(COLOR_PAIR(2));
                    }
                    addch(ACS_PLUS);
                    attroff(COLOR_PAIR(2));
                }
            }
            attron(COLOR_PAIR(1));
            addstr("  ");
            attroff(COLOR_PAIR(1));
            addch('\n');
        }
    }
    attron(COLOR_PAIR(1));
    addstr("                               ");
    attroff(COLOR_PAIR(1));
}

int updatett() {
    int x, y, f, xc, yc, i;
    xc = 0;
    yc = 0;
    for (x=0; x<10; x++) { /* Find full cols */
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
    for (y=0; y<10; y++) { /* Find full rows */
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
    for (i=0; i<xc; i++) /* Empty full cols */
    for (y=0; y<10; y++)
        wftt(xs[i], y);
    for (i=0; i<yc; i++) /* Empty full rows */
    for (x=0; x<10; x++)
        wftt(x, ys[i]);
    return 5*(xc+yc)*(xc+yc+1);
}
