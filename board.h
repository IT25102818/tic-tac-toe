#ifndef BOARD_H
#define BOARD_H

#include "types.h"               // use Game

char **allocBoard(int n);        // allocate n x n
void freeBoard(char **b, int n); // free memory
void clearBoard(Game *g);        // fill with '.'

void printBoard(const Game *g);              // show board
void openLog(Game *g, const char *filename); // open append log
void closeLog(Game *g);                      // close log
void logBoard(const Game *g);                // write board to log

#endif // BOARD_H
