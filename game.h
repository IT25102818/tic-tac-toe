#ifndef GAME_H
#define GAME_H

#include "types.h"                 

int isValidMove(const Game *g, int r, int c);     
void applyMove(Game *g, int r, int c, char sym);  
void humanMove(Game *g, int *outR, int *outC);   
int checkWin(const Game *g, char sym);            
int checkDraw(const Game *g);                     

void playGame(Game *g);                        

void setupTwoPlayer(Game *g);                     
void setupThreePlayer(Game *g);                   

#endif // GAME_H

