#include <stdio.h>   /* printf, putchar, fputs, puts */
#include <stdlib.h>  /* calloc, srand, rand */
#include <time.h>    /* time */
#include <ncurses.h>
#include "tt.c"
#include "bk.c"

/* TODO:
 * Either assert sizeof(int) == 4 or make portable code
 * Stop using char when int is good enough
 * Use better names
 * Stop “optimizing” with equivalent binary ops
 * Stop writing autistic code
 * Keep using gotos though, great shit
 */

int main(int argc, char **argv) {
    char f;      /* Flag */
    char c;      /* Input character / block number */
    char bks[3]; /* Blocks given to user */
    char bka[3]; /* Flags of blocks available ; TODO -> bitarray */
    char b;      /* Index of user chosen block (3 = no selection) */
    char oldb;   /* Previousy selected block, required for next/prev block */
    char sc;     /* Sub counter */
    char x, y;   /* Position of block */
    int score = 0;      /* Current score */
    int movepoints = 0; /* Points gained by the latest move */
    int totalmoves = 0; /* Number of moves made */
    tt = (int*)calloc(4, 4); /* !!! Hardcoded sizeof(int) = 4 */
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
    while (1) { /* Turns */
        if (!sc) {
            bks[0] = rand() % NBK; /* Pseudo-random ! */
            bks[1] = rand() % NBK;
            bks[2] = rand() % NBK;
            bka[0] = bka[1] = bka[2] = 1;
            b = 3; /* No block selected by default */
        }
        sc += 1;
        sc %= 3;
        /* Check if loose */
        for (c=0; c<3; c++) /* For each block */
            if (bka[c]) /* If block is available */
                for (y=0; y<10; y++) /* Test every position */
                for (x=0; x<10; x++)
                    if (bkfits(bks[c], x, y) == 1)
                        goto fits;
        attron(COLOR_PAIR(3));
        mvaddstr(3, 4, "            ");
        mvaddstr(4, 4, "  YOU LOSE  ");
        mvaddstr(5, 4, "            ");
        attroff(COLOR_PAIR(3));
        getch();
        break;
fits: /* Hasn't lost yet */
        f = 0; /* Valid play flag */
        x = y = 5; /* Default position */
        while (1) { /* Keypresses */
            clear();
            printtt();
            mvprintw(3, 40, "SCORE: %d", score);
            mvprintw(4, 46, "+%d", movepoints);
            mvprintw(5, 40, "MOVES MADE: %d", totalmoves);
            mvaddch(11, 36, '1');
            mvaddch(11, 51, '2');
            mvaddch(11, 66, '3');
            if (bka[0]) printbk(bks[0], 11, 6, 3);
            if (bka[1]) printbk(bks[1], 16, 6, 4);
            if (bka[2]) printbk(bks[2], 21, 6, 5);
            if (b < 3)  printbk(bks[b], x, y, b+3);
            refresh();
            c = getch();
            switch (c) {
              case '1' :
              case '2' :
              case '3' :
                c -= '1'; /* 0, 1 or 2 */
                if (!bka[c]) break;
                b = c;
                x = y = 5;
                break;
              /* Select previous block */
              case 44 : /* , */
              case 60 : /* < */
                oldb = b;
                if (b < 3) { /* If some block is selected */
                    b += 2;
                } else {
                    b = 2;
                }
                while (!bka[b%3]) b--;
                b %= 3;
                if (oldb != b) x = y = 5;
                break;
              /* Select next block */
              case 46 : /* . */
              case 62 : /* > */
                oldb = b;
                if (b < 3) { /* If some block is selected */
                    b++;
                } else {
                    b = 0;
                }
                while (!bka[b%3]) b++;
                b %= 3;
                if (oldb != b) x = y = 5;
                break;
              /* Left */
              case 4 :
                if (b < 3 && x > 0
                 && bkfits(bks[b], x-1, y) != -1)
                    x--;
                break;
              /* Down */
              case 2 :
                if (b < 3 && y < 9
                 && bkfits(bks[b], x, y+1) != -1)
                    y++;
                break;
              /* Up */
              case 3 :
                if (b < 3 && y > 0
                 && bkfits(bks[b], x, y-1) != -1)
                    y--;
                break;
              /* Right */
              case 5 :
                if (b < 3 && x < 9
                 && bkfits(bks[b], x+1, y) != -1)
                    x++;
                break;
              case 10 : /* Enter */
              case ' ' :
                if (b < 3 && bkfits(bks[b], x, y) == 1)
                    f = 1;
                break;
              case 27 : /* Escape */
              case 'q' :
                endwin();
                free(tt);
                return 0;
            }
            if (f) break;
        }
        bkinsert(bks[b], x, y);
        bka[b] = 0; /* Block b not available */
        /* No default selection unless only one block left */
        if (bka[0] + bka[1] + bka[2] == 1) { /* only one left */
            for (c=0; c<3; c++) { /* Find which one */
                if (bka[c]) {
                    b = c; /* Select the last remaining block */
                }
            }
        } else {
            b = 3; /* No selection made */
        }
        /* Score is calculated from number of squares in the block
         * and the number of rows+columns removed */
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
