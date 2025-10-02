#include <stdio.h>                  // printf, scanf
#include "game.h"                   // prototypes
#include "io.h"                     // prompts (safeReadInt used elsewhere)
#include "board.h"                  // printBoard, logBoard
#include "ai.h"                     // computerMoveRandom

int isValidMove(const Game *g, int r, int c) {           // check move
    if (r < 0 || r >= g->n) return 0;                    // row oob
    if (c < 0 || c >= g->n) return 0;                    // col oob
    if (g->board[r][c] != '.') return 0;                 // not empty
    return 1;                                            // valid
}

void applyMove(Game *g, int r, int c, char sym) {        // place symbol
    g->board[r][c] = sym;                                 // write cell
    g->moveCount++;                                       // count move
}

void humanMove(Game *g, int *outR, int *outC) {          // ask user
    int r, c;                                             // 1-based input
    while (1) {                                           // repeat until ok
        printf("Player %d ('%c') - row (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n); // prompt row
        scanf("%d", &r);                                  // read row
        printf("Player %d ('%c') - col (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n); // prompt col
        scanf("%d", &c);                                  // read col
        r--; c--;                                         // to 0-based
        if (isValidMove(g, r, c)) { *outR = r; *outC = c; return; } // ok
        printf("Invalid move. Try again.\n");             // not ok
    }
}

int checkWin(const Game *g, char sym) {                  // 4 directions
    int n = g->n;                                        // size
    for (int i = 0; i < n; i++) {                        // rows
        int ok = 1;                                      // assume match
        for (int j = 0; j < n; j++) if (g->board[i][j] != sym) { ok = 0; break; }
        if (ok) return 1;                                // row win
    }
    for (int j = 0; j < n; j++) {                        // cols
        int ok = 1;                                      // assume
        for (int i = 0; i < n; i++) if (g->board[i][j] != sym) { ok = 0; break; }
        if (ok) return 1;                                // col win
    }
    {                                                    // main diag
        int ok = 1;
        for (int i = 0; i < n; i++) if (g->board[i][i] != sym) { ok = 0; break; }
        if (ok) return 1;
    }
    {                                                    // anti diag
        int ok = 1;
        for (int i = 0; i < n; i++) if (g->board[i][n-1-i] != sym) { ok = 0; break; }
        if (ok) return 1;
    }
    return 0;                                            // no win
}

int checkDraw(const Game *g) {                           // full board?
    for (int i = 0; i < g->n; i++)                       // rows
        for (int j = 0; j < g->n; j++)                   // cols
            if (g->board[i][j] == '.') return 0;         // empty exists
    return 1;                                            // no empties
}

void playGame(Game *g) {                                 // main loop
    int r = -1, c = -1;                                  // move coords
    while (1) {                                          // run until break
        printBoard(g);                                   // show board

        if (g->types[g->current] == HUMAN) {             // human turn?
            humanMove(g, &r, &c);                        // ask for move
        } else {                                         // computer turn
            computerMoveRandom(g, &r, &c);               // random valid
            printf("Computer (Player %d '%c') chose: %d %d\n",
                   g->current + 1, g->symbols[g->current], r + 1, c + 1); // echo
        }

        applyMove(g, r, c, g->symbols[g->current]);      // place symbol
        logBoard(g);                                     // log board

        if (checkWin(g, g->symbols[g->current])) {       // win?
            printBoard(g);                               // final board
            printf("Player %d ('%c') WINS!\n",
                   g->current + 1, g->symbols[g->current]); // announce
            break;                                       // stop
        }

        if (checkDraw(g)) {                              // draw?
            printBoard(g);                               // final board
            printf("It's a DRAW!\n");                    // announce
            break;                                       // stop
        }

        g->current = (g->current + 1) % g->numPlayers;   // next player
    }
}

void setupTwoPlayer(Game *g) {                           // 2-player
    g->numPlayers = 2;                                   // two players
    g->symbols[0] = 'X';                                 // P1
    g->symbols[1] = 'O';                                 // P2
    g->symbols[2] = 'Z';                                 // spare
    g->types[0]   = HUMAN;                               // P1 human
    g->types[1]   = HUMAN;                               // P2 human (can switch)
    g->types[2]   = HUMAN;                               // spare
    g->current    = 0;                                   // P1 starts
    g->moveCount  = 0;                                   // reset count
}

void setupThreePlayer(Game *g) {                         // 3-player
    g->numPlayers = 3;                                   // three players
    g->symbols[0] = 'X';                                 // symbols
    g->symbols[1] = 'O';
    g->symbols[2] = 'Z';
    g->types[0]   = HUMAN;                               // default types
    g->types[1]   = HUMAN;
    g->types[2]   = HUMAN;                               // you can flip to COMPUTER if you want
    g->current    = 0;                                   // P1 starts
    g->moveCount  = 0;                                   // reset
}
