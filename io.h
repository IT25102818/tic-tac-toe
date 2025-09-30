#ifndef IO_H
#define IO_H

int safeReadInt(const char *prompt, int min, int max); // bounded int read
void pressEnterToContinue(void);                       // optional pause

#endif // IO_H
