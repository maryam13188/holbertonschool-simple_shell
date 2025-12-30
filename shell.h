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

/* Functions */
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
void free_args(char **args);
int is_whitespace(const char *str);
int handle_builtin(char **args);
void print_environment(void);
char *_strdup(const char *str);
int _strcmp(const char *s1, const char *s2);
char *find_command_in_path(char *command);
int command_exists(char *command);

#endif
