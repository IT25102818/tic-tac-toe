#include <stdio.h>                
#include "io.h"                   

int safeReadInt(const char *prompt, int min, int max) {
    int x;                        
    int ok = 0;                   
    while (!ok) {                 
        printf("%s", prompt);     
        if (scanf("%d", &x) == 1) {            
            if (x >= min && x <= max) ok = 1;  
            else printf("Enter a number between %d and %d.\n", min, max);
        } else {                                
            printf("Invalid input. Please enter a number.\n");
            int ch;                             
            while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        }
    }
    return x;                     
}

void pressEnterToContinue(void) { 
    int ch;                       
    printf("Press Enter to continue...");
    while ((ch = getchar()) != '\n' && ch != EOF) { /* flush */ }
    getchar();                    
}
