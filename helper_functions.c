#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* ---------------- String & Utils ---------------- */
char *_strdup(char *str)
{
char *dup;
if (!str) return NULL;
dup = malloc(strlen(str) + 1);
if (!dup) return NULL;
strcpy(dup, str);
return dup;
}
int _strcmp(char *s1, char *s2)
{
while (*s1 && *s2 && *s1 == *s2)
s1++, s2++;
return (*s1 - *s2);
}
void print_error(char *cmd)
{
if (!cmd) return;
write(2, cmd, strlen(cmd));
write(2, ": not found\n", 12);
}
/* ---------------- Input / Tokenization ---------------- */
char *read_line(void)
{
char *line = NULL;
size_t bufsize = 0;
if (getline(&line, &bufsize, stdin) == -1)
{
free(line);
return NULL;
}
return line;
}
char **split_line(char *line)
{
int bufsize = 64, position = 0;
char **tokens = malloc(bufsize * sizeof(char *));
char *token;
if (!tokens) return NULL;
token = strtok(line, " \t\r\n");
while (token)
{
tokens[position++] = _strdup(token);
if (position >= bufsize)
{
bufsize += 64;
tokens = realloc(tokens, bufsize * sizeof(char *));
if (!tokens) return NULL;
}
token = strtok(NULL, " \t\r\n");
}
tokens[position] = NULL;
return tokens;
}
void free_tokens(char **tokens)
{
int i = 0;
if (!tokens) return;
while (tokens[i])
{
free(tokens[i]);
i++;
}
free(tokens);
}
/* ---------------- PATH & Execution ---------------- */
char *get_path_from_environ(void)
{
extern char **environ;
int i = 0;
char *prefix = "PATH=";
if (!environ) return NULL;
while (environ[i])
{
if (strncmp(environ[i], prefix, 5) == 0)
return (environ[i] + 5);
i++;
}
return NULL;
}
int check_command(char *command)
{
struct stat st;
char *path_copy, *dir, *full_path;
char *path = get_path_from_environ();
if (!command || command[0] == '\0') return 0;
if (strchr(command, '/'))
return (stat(command, &st) == 0 && S_ISREG(st.st_mode));
if (!path) return 0;
path_copy = _strdup(path);
if (!path_copy) return 0;
dir = strtok(path_copy, ":");
while (dir)
{
full_path = malloc(strlen(dir) + strlen(command) + 2);
if (!full_path)
{
free(path_copy);
return 0;
}
sprintf(full_path, "%s/%s", dir, command);
if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
{
free(full_path);
free(path_copy);
return 1;
}
free(full_path);
dir = strtok(NULL, ":");
}
free(path_copy);
return 0;
}
char *find_full_path(char *command)
{
struct stat st;
char *path_copy, *dir, *full_path;
char *path = get_path_from_environ();
if (!command || command[0] == '\0') return NULL;
if (strchr(command, '/'))
return (stat(command, &st) == 0 && S_ISREG(st.st_mode)) ? _strdup(command) : NULL;
if (!path) return NULL;
path_copy = _strdup(path);
if (!path_copy) return NULL;
dir = strtok(path_copy, ":");
while (dir)
{
full_path = malloc(strlen(dir) + strlen(command) + 2);
if (!full_path)
{
free(path_copy);
return NULL;
}
sprintf(full_path, "%s/%s", dir, command);
if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
{
free(path_copy);
return full_path;
}
free(full_path);
dir = strtok(NULL, ":");
}
free(path_copy);
return NULL;
}
int execute(char **args)
{
pid_t pid;
int status;
char *full_path;
if (!args || !args[0] || args[0][0] == '\0')
return 1;
if (!check_command(args[0]))
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return 127;  
}
full_path = find_full_path(args[0]);
if (!full_path)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return 127;
}
pid = fork();
if (pid == 0)
{
execve(full_path, args, environ);
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
free(full_path);
exit(127);
}
else if (pid < 0)
{
perror("fork");
free(full_path);
return 1;
}
else
{
waitpid(pid, &status, 0);
free(full_path);
}
return 1;
}
