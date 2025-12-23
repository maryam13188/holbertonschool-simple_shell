#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"

char *read_line(void);
char **split_line(char *line);
int execute(char **args);
void free_tokens(char **tokens);

int _strlen(char *s);
char *_strdup(char *str);

#endif
