#ifndef TYPES_H                  
#define TYPES_H

#include <stdio.h>               
typedef enum {                   
    HUMAN = 0,                   
    COMPUTER = 1                 
} PlayerType;

typedef struct {                 
    int n;                       
    char **board;                
    int current;                 
    int numPlayers;              
    char symbols[3];             
    PlayerType types[3];         
    FILE *logf;                  
    int moveCount;               
} Game;

#endif 
