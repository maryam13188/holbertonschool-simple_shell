#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/* shell.c */
void shell_loop(void);

/* helper_functions.c */
char *_getenv(char *name);
char *find_path(char *command);
char **split_line(char *line);

#endif

