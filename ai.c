#include <stdlib.h>             
#include "ai.h"                   

void computerMoveRandom(Game *g, int *outR, int *outC) {
    int emptyCount = 0;                                               
    for (int i = 0; i < g->n; i++)                                    
        for (int j = 0; j < g->n; j++)                                 
            if (g->board[i][j] == '.') emptyCount++;                   

    if (emptyCount == 0) { *outR = *outC = 0; return; }               

    int pick = rand() % emptyCount;                                    
    int seen = 0;                                                      
    for (int i = 0; i < g->n; i++)                                     
        for (int j = 0; j < g->n; j++)                                 
            if (g->board[i][j] == '.') {                               
                if (seen == pick) { *outR = i; *outC = j; return; }    
                seen++;                                                
            }
}
