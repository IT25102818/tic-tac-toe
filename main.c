#include <stdio.h>               
#include <stdlib.h>               
#include <time.h>                 
#include "types.h"                
#include "io.h"                  
#include "board.h"                
#include "game.h"                 

int main(void) {                                           
    srand((unsigned)time(NULL));                         

    Game g;                                                
    g.logf = NULL;                                         

    g.n = safeReadInt("Enter board size N (3..10): ", 3, 10); 
    g.board = allocBoard(g.n);                             
    if (!g.board) { printf("Memory allocation failed.\n"); return EXIT_FAILURE; }

    clearBoard(&g);                                        
    openLog(&g, "game_log.txt");                         

    printf("\nChoose mode:\n");                          
    printf("  1) Human vs Human (2 players)\n");
    printf("  2) Human vs Computer (2 players)\n");
    printf("  3) Three Players (X, O, Z)\n");
    int mode = safeReadInt("Select 1..3: ", 1, 3);        

    if (mode == 1) {                                       
        setupTwoPlayer(&g);                                
    } else if (mode == 2) {                               
        setupTwoPlayer(&g);                             
        g.types[1] = COMPUTER;                             
    } else {                                              
        setupThreePlayer(&g);                              
        
    }

    playGame(&g);                                          

    closeLog(&g);                                          
    freeBoard(g.board, g.n);                               
    return EXIT_SUCCESS;                                   
}
