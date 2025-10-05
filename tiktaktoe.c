#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*  TYPE DEFINITIONS  */

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
    int i, j;
    if (!g->logf) return;
   
    fprintf(g->logf, "Move #%d (Player %d '%c')\n",
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
    fflush(g->logf);
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
        printf("Player %d ('%c') - row (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n);
        scanf("%d", &r);
        printf("Player %d ('%c') - col (1..%d): ",
               g->current + 1, g->symbols[g->current], g->n);
        scanf("%d", &c);
       
        r--; c--;
       
        if (isValidMove(g, r, c)) {
            *outR = r;
            *outC = c;
            return;
        }
        printf("Invalid move. Try again.\n");
    }
}

int checkWin(const Game *g, char sym) {
    int n = g->n;
    int i, j;
    int ok;
   
    /* Check rows */
    for (i = 0; i < n; i++) {
        ok = 1;
        for (j = 0; j < n; j++) {
            if (g->board[i][j] != sym) {
                ok = 0;
                break;
            }
        }
        if (ok) return 1;
    }
   
    /* Check columns */
    for (j = 0; j < n; j++) {
        ok = 1;
        for (i = 0; i < n; i++) {
            if (g->board[i][j] != sym) {
                ok = 0;
                break;
            }
        }
        if (ok) return 1;
    }
   
    /* Check main diagonal */
    ok = 1;
    for (i = 0; i < n; i++) {
        if (g->board[i][i] != sym) {
            ok = 0;
            break;
        }
    }
    if (ok) return 1;
   
    /* Check anti-diagonal */
    ok = 1;
    for (i = 0; i < n; i++) {
        if (g->board[i][n-1-i] != sym) {
            ok = 0;
            break;
        }
    }
    if (ok) return 1;
   
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
    g->types[0] = HUMAN;
    g->types[1] = HUMAN;
    g->types[2] = HUMAN;
    g->current = 0;
    g->moveCount = 0;
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
}

/* === MAIN FUNCTION ==== */

int main() {
    Game g;
    int mode;
   
    
    srand((unsigned)time(NULL));
   
    /* Initialize game structure */
    g.logf = NULL;
   
    printf("=== TIC-TAC-TOE GAME ===\n\n");
   
    /* Get board size from user */
    g.n = safeReadInt("Enter board size N (3..10): ", 3, 10);
   
    /* Allocate memory for board */
    g.board = allocBoard(g.n);
    if (!g.board) {
        printf("Memory allocation failed.\n");
        return 1;
    }
   
    
    clearBoard(&g);
    openLog(&g, "game_log.txt");
   
    
    printf("\nChoose game mode:\n");
    printf("  1) Human vs Human (2 players)\n");
    printf("  2) Human vs Computer (2 players)\n");
    printf("  3) Three Players (X, O, Z)\n");
    mode = safeReadInt("Select 1..3: ", 1, 3);
   
   
    if (mode == 1) {
        setupTwoPlayer(&g);
        printf("\nStarting 2-Player Human vs Human game...\n");
    } else if (mode == 2) {
        setupTwoPlayer(&g);
        g.types[1] = COMPUTER;
        printf("\nStarting Human vs Computer game...\n");
    } else {
        setupThreePlayer(&g);
        printf("\nStarting 3-Player game...\n");
    }
   
    printf("Game started! Good luck!\n");
    pressEnterToContinue();
   
    /* Start the main game loop */
    playGame(&g);
   
    /* Cleanup */
    closeLog(&g);
    freeBoard(g.board, g.n);
   
    printf("\nThank you for playing!\n");
    printf("Game log saved to 'game_log.txt'\n");
   
    return 0;
}


