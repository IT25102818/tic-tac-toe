#include <stdlib.h>              
#include <time.h>                
#include <stdio.h>               
#include "board.h"               

char **allocBoard(int n) {                           
    char **b = (char **)malloc(n * sizeof(char*));   
    if (!b) return NULL;                             
    for (int i = 0; i < n; i++) {                    
        b[i] = (char *)malloc(n * sizeof(char));     
        if (!b[i]) {                                 
            for (int k = 0; k < i; k++) free(b[k]);  
            free(b);                                 
            return NULL;                             
        }
    }
    return b;                                        
}

void freeBoard(char **b, int n) {                    
    if (!b) return;                                 
    for (int i = 0; i < n; i++) free(b[i]);          
    free(b);                                       
}

void clearBoard(Game *g) {                           
    for (int i = 0; i < g->n; i++)                  
        for (int j = 0; j < g->n; j++)               
            g->board[i][j] = '.';                    
}

void printBoard(const Game *g) {                     
    printf("\n   ");                                
    for (int j = 0; j < g->n; j++)                   
        printf("%2d ", j + 1);                       
    printf("\n");                                    
    for (int i = 0; i < g->n; i++) {                 
        printf("%2d ", i + 1);                       
        for (int j = 0; j < g->n; j++)              
            printf(" %c ", g->board[i][j]);          
        printf("\n");                                
    }
    printf("\n");                                    
}

void openLog(Game *g, const char *filename) {        
    g->logf = fopen(filename, "a");                  
    if (!g->logf) {                                  
        printf("Warning: couldn't open log '%s'\n", filename);
        return;                                      
    }
    time_t now = time(NULL);                         
    fprintf(g->logf, "---- New Game: %s", ctime(&now)); 
    fflush(g->logf);                                
}

void closeLog(Game *g) {                             
    if (!g->logf) return;                            
    fprintf(g->logf, "---- End of Game ----\n\n");   
    fclose(g->logf);                                 
    g->logf = NULL;                                  
}

void logBoard(const Game *g) {                       
    if (!g->logf) return;                            
    fprintf(g->logf, "Move #%d (Player %d '%c')\n",
            g->moveCount, g->current + 1, g->symbols[g->current]);
    fprintf(g->logf, "   ");                         
    for (int j = 0; j < g->n; j++)                   
        fprintf(g->logf, "%2d ", j + 1);
    fprintf(g->logf, "\n");                          
    for (int i = 0; i < g->n; i++) {                 
        fprintf(g->logf, "%2d ", i + 1);             
        for (int j = 0; j < g->n; j++)             
            fprintf(g->logf, " %c ", g->board[i][j]); 
        fprintf(g->logf, "\n");                     
    }
    fprintf(g->logf, "\n");                          
    fflush(g->logf);                                 
}

