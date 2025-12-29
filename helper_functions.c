#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* String */
char *_strdup(char *str)
{
    char *dup; if (!str) return NULL;
    dup = malloc(strlen(str) + 1);
    if (!dup) return NULL;
    strcpy(dup, str); return dup;
}

int _strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;
    return *s1 - *s2;
}

/* Input */
char *read_line(void)
{
    char *line = NULL; size_t bufsize = 0;
    if (isatty(0)) write(1, "$ ", 2);
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (isatty(0)) write(1, "\n", 1);
        free(line); return NULL;
    }
    if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
    return line;
}

char **split_line(char *line)
{
    int bufsize = 64, i = 0;
    char **t = malloc(bufsize * sizeof(char *));
    char *tok; if (!t) return NULL;
    tok = strtok(line, " \t\r\n");
    while (tok)
    {
        if (strlen(tok) > 0) t[i++] = _strdup(tok);
        if (i >= bufsize)
        {
            bufsize += 64;
            t = realloc(t, bufsize * sizeof(char *));
            if (!t) return NULL;
        }
        tok = strtok(NULL, " \t\r\n");
    }
    t[i] = NULL; return t;
}

void free_tokens(char **t)
{
    int i = 0; if (!t) return;
    while (t[i]) free(t[i++]);
    free(t);
}

/* PATH */
char *_getenv(const char *name)
{
    int i = 0; size_t len;
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

char *find_command_in_path(char *cmd)
{
    char *path, *copy, *dir, *full;
    struct stat st; if (!cmd) return NULL;
    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
            return _strdup(cmd);
        return NULL;
    }
    path = _getenv("PATH"); if (!path) return NULL;
    copy = _strdup(path); if (!copy) return NULL;
    dir = strtok(copy, ":");
    while (dir)
    {
        full = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full) { free(copy); return NULL; }
        sprintf(full, "%s/%s", dir, cmd);
        if (stat(full, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
        {
            free(copy); return full;
        }
        free(full); dir = strtok(NULL, ":");
    }
    free(copy); return NULL;
}

int check_command_exists(char *cmd)
{
    struct stat st; char *full;
    if (!cmd || cmd[0] == '\0') return 0;
    if (strchr(cmd, '/'))
        return (stat(cmd, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR));
    full = find_command_in_path(cmd);
    if (full) { free(full); return 1; }
    return 0;
}

/* Built-in */
int is_builtin(char *cmd)
{
    if (!cmd) return 0;
    return (_strcmp(cmd, "exit") == 0 || _strcmp(cmd, "env") == 0);
}

void handle_builtin(char **args)
{
    int i;
    if (_strcmp(args[0], "exit") == 0)
    {
        free_tokens(args); exit(0);
    }
    else if (_strcmp(args[0], "env") == 0)
    {
        i = 0; while (environ[i]) printf("%s\n", environ[i++]);
    }
}

/* Execute */
int execute_command(char **args)
{
    pid_t pid; int s; char *full; struct stat st;
    if (!args || !args[0]) return 1;
    if (is_builtin(args[0])) { handle_builtin(args); return 0; }
    if (!check_command_exists(args[0]))
    {
        fprintf(stderr, "%s: command not found\n", args[0]); return 127;
    }
    if (strchr(args[0], '/')) full = _strdup(args[0]);
    else full = find_command_in_path(args[0]);
    if (!full || stat(full, &st) == -1 || !S_ISREG(st.st_mode) || !(st.st_mode & S_IXUSR))
    {
        if (full) free(full);
        fprintf(stderr, "%s: command not found\n", args[0]); return 127;
    }
    pid = fork();
    if (pid == 0)
    {
        if (execve(full, args, environ) == -1)
        {
            perror(args[0]); free(full); exit(127);
        }
    }
    else if (pid < 0) { perror("fork"); free(full); return 1; }
    else
    {
        waitpid(pid, &s, 0); free(full);
        return WIFEXITED(s) ? WEXITSTATUS(s) : 1;
    }
    return 1;
}
