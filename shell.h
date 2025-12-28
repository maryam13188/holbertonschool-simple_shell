#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
/* ----------------- Core Shell ----------------- */
void shell_loop(void);
int execute(char **args);
/* ----------------- Input / Tokenization ----------------- */
char *read_line(void);
char **split_line(char *line);
void free_tokens(char **tokens);
/* ----------------- String Utilities ----------------- */
int _strlen(char *s);
char *_strdup(char *str);
int _strcmp(char *s1, char *s2);
/* ----------------- PATH / Command ----------------- */
char *get_path_from_environ(void);
int check_command(char *command);
char *find_full_path(char *command);
/* ----------------- Utils ----------------- */
void print_error(char *cmd);
/* ----------------- Environment ----------------- */
extern char **environ;
#endif /* SHELL_H */
