#include <stdio.h>

/* handler.s */
extern void setup_doscall_handler(void);
extern void restore_doscall_handler(void);

/* argument.c */
extern char *Format_output(int, void *);

/* display.c */
extern void display1(int, void *);
extern void display2(int);

/* trace.c */
extern FILE *Stream;
extern int Count;
extern char Option_A_flag;

// version.c
extern const char Title[];
