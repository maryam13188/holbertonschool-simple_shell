#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* String Utilities */
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
    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;
    return *s1 - *s2;
}

/* Input / Tokenization */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    
    if (isatty(STDIN_FILENO)) write(STDOUT_FILENO, "$ ", 2);
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (isatty(STDIN_FILENO)) write(STDOUT_FILENO, "\n", 1);
        free(line);
        return NULL;
    }
    if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
    return line;
}

char **split_line(char *line)
{
    int bufsize = 64, i = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    
    if (!tokens) return NULL;
    token = strtok(line, " \t\r\n");
    while (token)
    {
        if (strlen(token) > 0) tokens[i++] = _strdup(token);
        if (i >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) return NULL;
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[i] = NULL;
    return tokens;
}

void free_tokens(char **tokens)
{
    int i = 0;
    if (!tokens) return;
    while (tokens[i]) free(tokens[i++]);
    free(tokens);
}

/* PATH Handling */
char *_getenv(const char *name)
{
    int i = 0;
    size_t len;
    if (!environ || !name) return NULL;
    len = strlen(name);
    while (environ[i])
    {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
            return (environ[i] + len + 1);
        i++;
    }
    return NULL;
}

char *find_command_in_path(char *command)
{
    char *path, *path_copy, *dir, *full_path;
    struct stat st;
    
    if (!command) return NULL;
    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
            return _strdup(command);
        return NULL;
    }
    
    path = _getenv("PATH");
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
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
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

int check_command_exists(char *command)
{
    struct stat st;
    char *full_path;
    
    if (!command || command[0] == '\0') return 0;
    if (strchr(command, '/'))
        return (stat(command, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR));
    
    full_path = find_command_in_path(command);
    if (full_path)
    {
        free(full_path);
        return 1;
    }
    return 0;
}

/* Command Execution */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *full_path;
    struct stat st;
    
    if (!args || !args[0]) return 1;
    if (!check_command_exists(args[0]))
    {
        fprintf(stderr, "%s: command not found\n", args[0]);
        return 127;
    }
    
    if (strchr(args[0], '/'))
        full_path = _strdup(args[0]);
    else
        full_path = find_command_in_path(args[0]);
    
    if (!full_path || stat(full_path, &st) == -1 || 
        !S_ISREG(st.st_mode) || !(st.st_mode & S_IXUSR))
    {
        if (full_path) free(full_path);
        fprintf(stderr, "%s: command not found\n", args[0]);
        return 127;
    }
    
    pid = fork();
    if (pid == 0)
    {
        if (execve(full_path, args, environ) == -1)
        {
            perror(args[0]);
            free(full_path);
            exit(127);
        }
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
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    return 1;
}
