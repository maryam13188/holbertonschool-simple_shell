#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/* main shell */
int execute_cmd(char **argv);

/* helpers */
char *read_line(void);
char **tokenize(char *line);
char *get_path(char *cmd);
void print_error(char *cmd);

#endif

