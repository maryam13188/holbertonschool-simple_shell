#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Helper functions */
char *get_path(char *command);
char **split_string(char *str, const char *delim);
void free_tokens(char **tokens);
int execute_command(char **args);
void handle_path_and_execute(char **args);
int is_builtin(char **args);
void handle_builtin(char **args);

/* Shell main functions */
void shell_loop(void);
char *read_line(void);
char **parse_line(char *line);

#endif
