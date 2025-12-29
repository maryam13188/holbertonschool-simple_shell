#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;

/* Function prototypes */
void shell_loop(void);
char *_strdup(char *str);
int _strcmp(char *s1, char *s2);
char *read_line(void);
char **split_line(char *line);
void free_tokens(char **tokens);
char *find_command_in_path(char *command);
int execute_command(char **args);
int check_command_exists(char *command);
int execute_command(char **args);
int is_builtin(char *command);
void handle_builtin(char **args);
#endif
