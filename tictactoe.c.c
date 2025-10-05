
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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

/* ========== FUNCTION IMPLEMENTATIONS ========== */


int safeReadInt(const char *prompt, int min, int max) {
    int x;
    int ok = 0;
    int ch;
    while (!ok) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) {
            if (x >= min && x <= max) {
                ok = 1;
            } else {
                printf("Enter a number between %d and %d.\n", min, max);
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* clear input buffer */
            }
        }
    }
    return x;
}

void pressEnterToContinue(void) {
    int ch;
    printf("Press Enter to continue...");
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* flush input buffer */
    }
}

/* Board Functions */
char **allocBoard(int n) {
    char **b;
    int i, k;
    
    b = (char **)malloc(n * sizeof(char*));
    if (!b) return NULL;
    
    for (i = 0; i < n; i++) {
        b[i] = (char *)malloc(n * sizeof(char));
        if (!b[i]) {
            for (k = 0; k < i; k++) {
                free(b[k]);
            }
            free(b);
            return NULL;
        }
    }
    return b;
}

void freeBoard(char **b, int n) {
    int i;
    if (!b) return;
    for (i = 0; i < n; i++) {
        free(b[i]);
    }
    free(b);
}

void clearBoard(Game *g) {
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            g->board[i][j] = '.';
        }
    }
}

void printBoard(const Game *g) {
    int i, j;
    printf("\n   ");
    for (j = 0; j < g->n; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");
    
    for (i = 0; i < g->n; i++) {
        printf("%2d ", i + 1);
        for (j = 0; j < g->n; j++) {
            printf(" %c ", g->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void openLog(Game *g) {
    /* Always use the same filename */
    g->logf = fopen("tictactoe_log.txt", "a");  /* CHANGED TO "a" for APPEND */
    if (!g->logf) {
        printf("Error: Could not open log file 'tictactoe_log.txt'\n");
        return;
    }
    
    time_t now = time(NULL);
    fprintf(g->logf, "\n\n========================================\n");
    fprintf(g->logf, "        NEW GAME STARTED\n");
    fprintf(g->logf, "========================================\n");
    fprintf(g->logf, "Game Time: %s", ctime(&now));
    fprintf(g->logf, "Board Size: %dx%d\n", g->n, g->n);
    fprintf(g->logf, "========================================\n\n");
    fflush(g->logf);
    
    printf("=== LOGGING STARTED ===\n");
    printf("All moves will be saved to: tictactoe_log.txt\n");
}

void closeLog(Game *g) {
    if (!g->logf) return;
    
    time_t now = time(NULL);
    fprintf(g->logf, "========================================\n");
    fprintf(g->logf, "         GAME ENDED\n");
    fprintf(g->logf, "End Time: %s", ctime(&now));
    fprintf(g->logf, "Total Moves: %d\n", g->moveCount);
    fprintf(g->logf, "========================================\n");
    fclose(g->logf);
    g->logf = NULL;
}

void logBoard(const Game *g) {
    int i, j;
    if (!g->logf) return;
    
    fprintf(g->logf, "Move #%d - Player %d ('%c')\n", 
            g->moveCount, g->current + 1, g->symbols[g->current]);
    
    fprintf(g->logf, "   ");
    for (j = 0; j < g->n; j++) {
        fprintf(g->logf, "%2d ", j + 1);
    }
    fprintf(g->logf, "\n");
    
    for (i = 0; i < g->n; i++) {
        fprintf(g->logf, "%2d ", i + 1);
        for (j = 0; j < g->n; j++) {
            fprintf(g->logf, " %c ", g->board[i][j]);
        }
        fprintf(g->logf, "\n");
    }
    fprintf(g->logf, "\n");
    fflush(g->logf);  /* Force write after each move */
}

/* AI Functions */
void computerMoveRandom(Game *g, int *outR, int *outC) {
    int emptyCount = 0;
    int i, j;
    int pick, seen;
    
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            if (g->board[i][j] == '.') {
                emptyCount++;
            }
        }
    }

    if (emptyCount == 0) {
        *outR = 0;
        *outC = 0;
        return;
    }

    pick = rand() % emptyCount;
    seen = 0;
    
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            if (g->board[i][j] == '.') {
                if (seen == pick) {
                    *outR = i;
                    *outC = j;
                    return;
                }
                seen++;
            }
        }
    }
}

/* Game Logic Functions */
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
        printf("Player %d ('%c') - Enter row (1-%d): ",
               g->current + 1, g->symbols[g->current], g->n);
        if (scanf("%d", &r) != 1) {
            printf("Invalid input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }
        
        printf("Player %d ('%c') - Enter column (1-%d): ",
               g->current + 1, g->symbols[g->current], g->n);
        if (scanf("%d", &c) != 1) {
            printf("Invalid input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }
        
        r--; c--;
        
        if (isValidMove(g, r, c)) {
            *outR = r;
            *outC = c;
            return;
        }
        printf("Invalid move! That position is already taken or out of bounds.\n");
    }
}

int checkWin(const Game *g, char sym) {
    int n = g->n;
    int i, j;
    int win;
    
    for (i = 0; i < n; i++) {
        win = 1;
        for (j = 0; j < n; j++) {
            if (g->board[i][j] != sym) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    
    for (j = 0; j < n; j++) {
        win = 1;
        for (i = 0; i < n; i++) {
            if (g->board[i][j] != sym) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    
    win = 1;
    for (i = 0; i < n; i++) {
        if (g->board[i][i] != sym) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    win = 1;
    for (i = 0; i < n; i++) {
        if (g->board[i][n-1-i] != sym) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    return 0;
}

int checkDraw(const Game *g) {
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            if (g->board[i][j] == '.') return 0;
        }
    }
    return 1;
}

void playGame(Game *g) {
    int r = -1, c = -1;
    
    printf("\n=== GAME STARTED ===\n");
    
    while (1) {
        printf("\nCurrent Board:");
        printBoard(g);
        
        if (g->types[g->current] == HUMAN) {
            humanMove(g, &r, &c);
        } else {
            computerMoveRandom(g, &r, &c);
            printf("Computer (Player %d '%c') chose position: %d, %d\n",
                   g->current + 1, g->symbols[g->current], r + 1, c + 1);
        }
        
        applyMove(g, r, c, g->symbols[g->current]);
        printf("Player %d ('%c') placed at (%d, %d)\n", 
               g->current + 1, g->symbols[g->current], r + 1, c + 1);
        
        /* Log the move to file */
        logBoard(g);
        printf("? Move #%d saved to log file\n", g->moveCount);
        
        if (checkWin(g, g->symbols[g->current])) {
            printf("\n");
            printBoard(g);
            printf("*** PLAYER %d ('%c') WINS! ***\n", 
                   g->current + 1, g->symbols[g->current]);
            break;
        }
        
        if (checkDraw(g)) {
            printf("\n");
            printBoard(g);
            printf("*** IT'S A DRAW! ***\n");
            break;
        }
        
        g->current = (g->current + 1) % g->numPlayers;
    }
}

void setupTwoPlayer(Game *g) {
    g->numPlayers = 2;
    g->symbols[0] = 'X';
    g->symbols[1] = 'O';
    g->symbols[2] = '-';
    g->types[0] = HUMAN;
    g->types[1] = HUMAN;
    g->types[2] = HUMAN;
    g->current = 0;
    g->moveCount = 0;
    printf("Mode: 2 Players (X vs O)\n");
}

void setupThreePlayer(Game *g) {
    g->numPlayers = 3;
    g->symbols[0] = 'X';
    g->symbols[1] = 'O';
    g->symbols[2] = 'Z';
    g->types[0] = HUMAN;
    g->types[1] = HUMAN;
    g->types[2] = HUMAN;
    g->current = 0;
    g->moveCount = 0;
    printf("Mode: 3 Players (X, O, Z)\n");
}

/* ==MAIN FUNCTION  */

int main() {
    Game g;
    int mode;
    char playAgain;
    int gameCount = 0;
    
    printf("=========================================\n");
    printf("        TIC-TAC-TOE GAME\n");
    printf("  All Games Saved to One Log File\n");
    printf("=========================================\n\n");
    
    do {
        gameCount++;
        printf("\n=== GAME #%d ===\n", gameCount);
        
        
        srand((unsigned)time(NULL));
        
        
        g.logf = NULL;
        g.moveCount = 0;
        
        /* Get board size from user */
        g.n = safeReadInt("Enter board size (3-10): ", 3, 10);
        
        /* Allocate memory for board */
        g.board = allocBoard(g.n);
        if (!g.board) {
            printf("Error: Memory allocation failed!\n");
            return 1;
        }
        
        /* Initialize board */
        clearBoard(&g);
        
        
        openLog(&g);
        
        
        printf("\nSelect Game Mode:\n");
        printf("1. Human vs Human (2 players)\n");
        printf("2. Human vs Computer\n");
        printf("3. Three Players\n");
        mode = safeReadInt("Enter choice (1-3): ", 1, 3);
        
        
        if (mode == 1) {
            setupTwoPlayer(&g);
        } else if (mode == 2) {
            setupTwoPlayer(&g);
            g.types[1] = COMPUTER;
            printf("Mode: Human (X) vs Computer (O)\n");
        } else {
            setupThreePlayer(&g);
        }
        
        printf("\nGame is starting...\n");
        pressEnterToContinue();
        
        
        playGame(&g);
        
        /* Cleanup */
        closeLog(&g);
        freeBoard(g.board, g.n);
        
        printf("\n=========================================\n");
        printf("    GAME #%d COMPLETED!\n", gameCount);
        printf("All moves saved to: tictactoe_log.txt\n");
        printf("=========================================\n");
        
        
        printf("\nDo you want to play another game? (y/n): ");
        scanf(" %c", &playAgain);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF); /* Clear buffer */
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("\nThank you for playing %d games!\n", gameCount);
    printf("All games saved to: tictactoe_log.txt\n");
    printf("Goodbye!\n");
    
    return 0;
}
