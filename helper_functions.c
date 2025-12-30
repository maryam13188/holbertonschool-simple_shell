#include "shell.h"
extern char **environ;

char *get_path(char *cmd)
{
    char *path, *dir, *full;
    struct stat st;
    int i;

    if (!cmd || !cmd[0]) return (NULL);
    for (i = 0; cmd[i]; i++)
        if (cmd[i] == '/')
            return (stat(cmd, &st) == 0 && S_ISREG(st.st_mode) ? strdup(cmd) : NULL);

    path = getenv("PATH");
    if (!path || !path[0]) return (NULL);

    path = strdup(path);
    dir = strtok(path, ":");
    while (dir)
    {
        if (dir[0])
        {
            full = malloc(strlen(dir) + strlen(cmd) + 2);
            if (!full) { free(path); return (NULL); }
            sprintf(full, "%s/%s", dir, cmd);
            if (stat(full, &st) == 0 && S_ISREG(st.st_mode))
            {
                free(path);
                return (full);
            }
            free(full);
        }
        dir = strtok(NULL, ":");
    }
    free(path);
    return (NULL);
}

char **split_string(char *str, const char *delim)
{
    char **tokens, *token, *copy;
    int i = 0, count = 0;

    if (!str || !delim) return (NULL);
    copy = strdup(str);
    token = strtok(copy, delim);
    while (token) { if (token[0]) count++; token = strtok(NULL, delim); }
    free(copy);

    tokens = malloc(sizeof(char *) * (count + 1));
    copy = strdup(str);
    token = strtok(copy, delim);
    while (token)
    {
        if (token[0]) tokens[i++] = strdup(token);
        token = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    free(copy);
    return (tokens);
}

void free_tokens(char **tokens)
{
    int i;
    if (!tokens) return;
    for (i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}

int execute_command(char **args)
{
    pid_t pid;
    int status;

    if (!args || !args[0]) return (0);
    pid = fork();
    if (pid == -1) { perror("fork"); return (0); }
    if (pid == 0)
    {
        if (execve(args[0], args, environ) == -1)
        {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    waitpid(pid, &status, 0);
    return (1);
}

void handle_path_and_execute(char **args)
{
    char *full_path;
    struct stat st;

    if (!args || !args[0]) return;
    if (strchr(args[0], '/'))
    {
        if (stat(args[0], &st) == 0 && S_ISREG(st.st_mode))
            execute_command(args);
        else
            fprintf(stderr, "%s: command not found\n", args[0]);
    }
    else
    {
        full_path = get_path(args[0]);
        if (full_path)
        {
            free(args[0]);
            args[0] = full_path;
            execute_command(args);
        }
        else
            fprintf(stderr, "%s: command not found\n", args[0]);
    }
}

int is_builtin(char **args)
{
    if (!args || !args[0]) return (0);
    return (strcmp(args[0], "exit") == 0 || strcmp(args[0], "env") == 0);
}

void handle_builtin(char **args)
{
    char **env;
    if (!args || !args[0]) return;
    if (strcmp(args[0], "exit") == 0)
        exit(EXIT_SUCCESS);
    else if (strcmp(args[0], "env") == 0)
        for (env = environ; *env; env++) printf("%s\n", *env);
}
