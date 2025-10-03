#ifndef BOARD_H
#define BOARD_H

#include "types.h"               

char **allocBoard(int n);        
void freeBoard(char **b, int n); 
void clearBoard(Game *g);        

void printBoard(const Game *g);              
void openLog(Game *g, const char *filename); 
void closeLog(Game *g);                      
void logBoard(const Game *g);                

#endif // BOARD_H

