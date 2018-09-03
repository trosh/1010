/********** TT ************/
/* Grid of tiles for 1010 */
/**************************/

//Unicode block character
const cchar_t vert_sep_right = {A_NORMAL, 0x2590};
const cchar_t vert_sep_full = {A_NORMAL, 0x2588};

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
        attron(A_UNDERLINE);
        for (x=0; x<10; x++) {
            if (rtt(x, y)) {
                attron(COLOR_PAIR(2));
                addstr("  ");
                // Fill + Rightwards border = full block
                add_wch(&vert_sep_full);
                attroff(COLOR_PAIR(2));
            } else {
                addstr("  ");
                add_wch(&vert_sep_right);
            }
            attroff(COLOR_PAIR(2));
        }
        attroff(A_UNDERLINE);
        attron(COLOR_PAIR(1));
        addstr("  ");
        attroff(COLOR_PAIR(1));
        addch('\n');
    }
    attron(COLOR_PAIR(1));
    addstr("                                ");
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
