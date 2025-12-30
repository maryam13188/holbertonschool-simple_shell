#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Function Prototypes */
int is_whitespace(const char *str);
char *read_line(void);
char **split_line(char *line);
void free_tokens(char **tokens);
int execute(char **args);

/* String Functions */
int _strlen(char *s);
char *_strdup(char *str);
int _strcmp(char *s1, char *s2);

/* PATH Functions */
char *get_path_from_environ(void);
int check_command(char *command);
char *find_full_path(char *command);

/* Environment */
extern char **environ;

#endif

