#include <stdio.h>                   
#include "game.h"                   
#include "io.h"                    
#include "board.h"
#include "ai.h"                     

int isValidMove(const Game *g, int r, int c) {           
    if (r < 0 || r >= g->n) return 0;                   
    if (c < 0 || c >= g->n) return 0;                    
    if (g->board[r][c] != '.') return 0;                 
    return 1;                                            
}

void applyMove(Game *g, int r, int c, char sym) {        
    g->board[r][c] = sym;                                 
    g->moveCount++;
}

void humanMove(Game *g, int *outR, int *outC) {            
    int r, c;                                            
    while (1) {                                           
        printf("Player %d ('%c') - row (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n); 
        scanf("%d", &r);                                  
        printf("Player %d ('%c') - col (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n); 
        scanf("%d", &c);                                
        r--; c--;                                         
        if (isValidMove(g, r, c)) { *outR = r; *outC = c; return; } 
        printf("Invalid move. Try again.\n");             
    }
}

int checkWin(const Game *g, char sym) {                 
    int n = g->n;                                     
    for (int i = 0; i < n; i++) {                        
        int ok = 1;                                      
        for (int j = 0; j < n; j++) if (g->board[i][j] != sym) { ok = 0; break; }
        if (ok) return 1;                                
    }
    for (int j = 0; j < n; j++) {                        
        int ok = 1;                                      
        for (int i = 0; i < n; i++) if (g->board[i][j] != sym) { ok = 0; break; }
        if (ok) return 1;                               
    }
    {                                                   
        int ok = 1;
        for (int i = 0; i < n; i++) if (g->board[i][i] != sym) { ok = 0; break; }
        if (ok) return 1;
    }
    {                                                    
        int ok = 1;
        for (int i = 0; i < n; i++) if (g->board[i][n-1-i] != sym) { ok = 0; break; }
        if (ok) return 1;
    }
    return 0;                                           
}

int checkDraw(const Game *g) {                           
    for (int i = 0; i < g->n; i++)                       
        for (int j = 0; j < g->n; j++)                   
            if (g->board[i][j] == '.') return 0;         
    return 1;                                            
}

void playGame(Game *g) {                                 
    int r = -1, c = -1;                                  
    while (1) {                                          
        printBoard(g);                                  

        if (g->types[g->current] == HUMAN) {             
            humanMove(g, &r, &c);                        
        } else {                                         
            computerMoveRandom(g, &r, &c);         
            printf("Computer (Player %d '%c') chose: %d %d\n",
                   g->current + 1, g->symbols[g->current], r + 1, c + 1); 
        }

        applyMove(g, r, c, g->symbols[g->current]);      
        logBoard(g);                                     

        if (checkWin(g, g->symbols[g->current])) {       
            printBoard(g);                               
            printf("Player %d ('%c') WINS!\n",
                   g->current + 1, g->symbols[g->current]); 
            break;                                       
        }

        if (checkDraw(g)) {                              
            printBoard(g);                               
            printf("It's a DRAW!\n");                    
            break;                                       
        }

        g->current = (g->current + 1) % g->numPlayers;   
    }
}

void setupTwoPlayer(Game *g) {                           
    g->numPlayers = 2;                                  
    g->symbols[0] = 'X';                                 
    g->symbols[1] = 'O';                                
    g->symbols[2] = 'Z';                                 
    g->types[0]   = HUMAN;                               
    g->types[1]   = HUMAN;                               
    g->types[2]   = HUMAN;                          
    g->current    = 0;                                   
    g->moveCount  = 0;                                  
}

void setupThreePlayer(Game *g) {                         
    g->numPlayers = 3;                                   
    g->symbols[0] = 'X';                                 
    g->symbols[1] = 'O';
    g->symbols[2] = 'Z';
    g->types[0]   = HUMAN;                               
    g->types[1]   = HUMAN;
    g->types[2]   = HUMAN;                               
    g->current    = 0;                                  
    g->moveCount  = 0;                                  
}

