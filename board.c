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

void freeBoard(char **b, int n) {                    // free memory
    if (!b) return;                                  // nothing to free
    for (int i = 0; i < n; i++) free(b[i]);          // free rows
    free(b);                                         // free pointer array
}

void clearBoard(Game *g) {                           // set all to '.'
    for (int i = 0; i < g->n; i++)                   // rows
        for (int j = 0; j < g->n; j++)               // cols
            g->board[i][j] = '.';                    // empty cell
}

void printBoard(const Game *g) {                     // display board
    printf("\n   ");                                 // pad
    for (int j = 0; j < g->n; j++)                   // columns header
        printf("%2d ", j + 1);                       // 1..N
    printf("\n");                                    // newline
    for (int i = 0; i < g->n; i++) {                 // each row
        printf("%2d ", i + 1);                       // row index
        for (int j = 0; j < g->n; j++)               // each col
            printf(" %c ", g->board[i][j]);          // cell
        printf("\n");                                // end row
    }
    printf("\n");                                    // spacing
}

void openLog(Game *g, const char *filename) {        // open log
    g->logf = fopen(filename, "a");                  // append
    if (!g->logf) {                                  // if fail
        printf("Warning: couldn't open log '%s'\n", filename);
        return;                                      // continue without log
    }
    time_t now = time(NULL);                         // time
    fprintf(g->logf, "---- New Game: %s", ctime(&now)); // header
    fflush(g->logf);                                 // flush
}

void closeLog(Game *g) {                             // close log
    if (!g->logf) return;                            // nothing open
    fprintf(g->logf, "---- End of Game ----\n\n");   // footer
    fclose(g->logf);                                 // close
    g->logf = NULL;                                  // clear
}

void logBoard(const Game *g) {                       // write board
    if (!g->logf) return;                            // no log, skip
    fprintf(g->logf, "Move #%d (Player %d '%c')\n",
            g->moveCount, g->current + 1, g->symbols[g->current]); // header line
    fprintf(g->logf, "   ");                         // pad
    for (int j = 0; j < g->n; j++)                   // header
        fprintf(g->logf, "%2d ", j + 1);
    fprintf(g->logf, "\n");                          // newline
    for (int i = 0; i < g->n; i++) {                 // rows
        fprintf(g->logf, "%2d ", i + 1);             // row index
        for (int j = 0; j < g->n; j++)               // cols
            fprintf(g->logf, " %c ", g->board[i][j]); // cell
        fprintf(g->logf, "\n");                      // end row
    }
    fprintf(g->logf, "\n");                          // blank line
    fflush(g->logf);                                 // ensure write
}
