#ifndef GAME_H
#define GAME_H

#include "types.h"                 // Game

int isValidMove(const Game *g, int r, int c);     // bounds + empty
void applyMove(Game *g, int r, int c, char sym);  // write move
void humanMove(Game *g, int *outR, int *outC);    // prompt for move
int checkWin(const Game *g, char sym);            // N in a line?
int checkDraw(const Game *g);                     // full board?

void playGame(Game *g);                           // main loop

void setupTwoPlayer(Game *g);                     // 2 players (X/O)
void setupThreePlayer(Game *g);                   // 3 players (X/O/Z)

#endif // GAME_H
