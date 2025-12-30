#include "shell.h"

/* Declare environ */
extern char **environ;

/**
 * get_path - Finds full path of command
 * @cmd: Command entered
 * Return: Full path or NULL
 */
char *get_path(char *cmd)
{
    char *path, *dir, *full;
    struct stat st;
    int i;

    if (!cmd) return (NULL);

    for (i = 0; cmd[i]; i++)
        if (cmd[i] == '/')
            return (stat(cmd, &st) == 0 ? strdup(cmd) : NULL);

    path = getenv("PATH");
    if (!path) return (NULL);

    path = strdup(path);
    dir = strtok(path, ":");
    while (dir)
    {
        full = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full) { free(path); return (NULL); }
        sprintf(full, "%s/%s", dir, cmd);

        if (stat(full, &st) == 0)
        {
            free(path);
            return (full);
        }
        free(full);
        dir = strtok(NULL, ":");
    }
    free(path);
    return (NULL);
}

/**
 * split_string - Splits string into tokens
 * @str: String to split
 * @delim: Delimiters
 * Return: Array of tokens
 */
char **split_string(char *str, const char *delim)
{
    char **tokens = NULL, *token, *copy;
    int i = 0, count = 0;

    if (!str || !delim) return (NULL);

    copy = strdup(str);
    token = strtok(copy, delim);
    while (token) { count++; token = strtok(NULL, delim); }
    free(copy);

    tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens) return (NULL);

    copy = strdup(str);
    token = strtok(copy, delim);
    while (token)
    {
        tokens[i++] = strdup(token);
        token = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    free(copy);
    return (tokens);
}

/**
 * free_tokens - Frees tokens array
 * @tokens: Array to free
 */
void free_tokens(char **tokens)
{
    int i;
    if (!tokens) return;
    for (i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}

/**
 * execute_command - Executes command
 * @args: Command and args
 * Return: 1 on success, 0 on failure
 */
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

/**
 * handle_path_and_execute - Handles PATH and executes
 * @args: Command and args
 */
void handle_path_and_execute(char **args)
{
    char *full_path;
    struct stat st;

    if (!args || !args[0]) return;

    if (strchr(args[0], '/'))
    {
        if (stat(args[0], &st) == 0)
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

/**
 * is_builtin - Checks if built-in command
 * @args: Command and args
 * Return: 1 if built-in, 0 otherwise
 */
int is_builtin(char **args)
{
    if (!args || !args[0]) return (0);
    return (strcmp(args[0], "exit") == 0 || strcmp(args[0], "env") == 0);
}

/**
 * handle_builtin - Handles built-in commands
 * @args: Command and args
 */
void handle_builtin(char **args)
{
    char **env;

    if (!args || !args[0]) return;

    if (strcmp(args[0], "exit") == 0)
        exit(EXIT_SUCCESS);
    else if (strcmp(args[0], "env") == 0)
        for (env = environ; *env; env++) printf("%s\n", *env);
}
