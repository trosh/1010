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
#include "tt.c"
#include "bk.c"

/********** MAIN *********/

/**************************/
/*** TODO:              ***/
/*** AI                 ***/
/**************************/

int main(int argc, char **argv) {
    char f;      /* FLAG */
    char c;      /* INPUT CHARACTER / BLOCK NUMBER */
    char bks[3]; /* BLOCKS GIVEN TO USER */
    char bka[3]; /* FLAGS OF BLOCKS AVAILABLE ; TODO -> BITARRAY */
    char b;      /* INDEX OF USER CHOSEN BLOCK */
    char sc;     /* SUB COUNTER */
    char x, y;   /* POSITION OF BLOCK */
    int score = 0;      /* CURRENT SCORE */
    int movepoints = 0; /* POINTS GAINED BY THE LATEST MOVE */
    int totalmoves = 0; /* NUMBER OF MOVES MADE */
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
    while (1) { /* TURNS */
        if (!sc) {
            bks[0] = rand()%NBK; /* PSEUDO-RANDOM ! */
            bks[1] = rand()%NBK;
            bks[2] = rand()%NBK;
            bka[0] = bka[1] = bka[2] = 1;
            b = 0;
        }
        sc += 1;
        sc %= 3;
        /* CHECK IF LOOSE */
        for (c=0; c<3; c++) /* FOR EACH BLOCK */
            if (bka[c]) /* IF BLOCK IS AVAILABLE */
                for (y=0; y<10; y++) /* TEST EVERY POSITION */
                for (x=0; x<10; x++)
                    if (bkfits(bks[c], x, y) == 1)
                        goto fits; /* FITS */
        attron(COLOR_PAIR(3));
        mvaddstr(3, 4, "            ");
        mvaddstr(4, 4, "  YOU LOSE  ");
        mvaddstr(5, 4, "            ");
        attroff(COLOR_PAIR(3));
        getch();
        break;
fits: /* HASN'T LOST YET */
        f = 0; /* VALID PLAY FLAG */
        x = y = 5; /* DEFAULT POSITION */
        while (1) { /* KEYPRESSES */
            clear();
            printtt();
            mvprintw(3, 30, "Score: %d", score);
            mvprintw(4, 36, "+%d", movepoints);
            mvprintw(5, 30, "Moves made: %d", totalmoves);
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
            case 49 : /* 1 */
                if (!bka[0]) break;
                b = 0;
                x = y = 5;
                break;
            case 50 : /* 2 */
                if (!bka[1]) break;
                b = 1;
                x = y = 5;
                break;
            case 51 : /* 3 */
                if (!bka[2]) break;
                b = 2;
                x = y = 5;
                break;
            case 4 : /* LEFT */
                if (x > 0
                 && bkfits(bks[b], x-1, y) != -1)
                    x--;
                break;
            case 2 : /* DOWN */
                if (y < 9
                 && bkfits(bks[b], x, y+1) != -1)
                    y++;
                break;
            case 3 : /* UP */
                if (y > 0
                 && bkfits(bks[b], x, y-1) != -1)
                    y--;
                break;
            case 5 : /* RIGHT */
                if (x < 9
                 && bkfits(bks[b], x+1, y) != -1)
                    x++;
                break;
            case 10 : /* ENTER */
            case 32 : /* SPACE */
                if (bkfits(bks[b], x, y) == 1)
                    f = 1;
                break;
            case 27 : /* ESCAPE */
            case 'q' :
                endwin();
                free(tt);
                return 0;
            }
            if (f) break;
        }
        bkinsert(bks[b], x, y);
        bka[b] = 0; /* BLOCK B NOT AVAILABLE */
        for (c=0; c<3; c++) { /* CHOOSE NEW B */
            if (b == c /* DON'T CHECK BKA[B] */
             || !bka[c]) continue;
            b = c;
            break;
        }
        /* SCORE IS CALCULATED FROM NUMBER OF SQUARES IN THE BLOCK
         * AND THE NUMBER OF ROWS+COLUMNS REMOVED */
        movepoints = bksquares[bks[b]];
        movepoints += updatett();
        score += movepoints;
        totalmoves++;
        if (!(bka[0]+bka[1]+bka[2])) continue;
    }
    endwin();
    free(tt);
    return 0;
}

