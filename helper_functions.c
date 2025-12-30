#include "shell.h"

char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, stdin) == -1)
    {
        free(line);
        return (NULL);
    }
    return (line);
}

char **tokenize(char *line)
{
    char **tokens;
    char *token;
    int i = 0;

    tokens = malloc(sizeof(char *) * 64);
    if (!tokens)
        return (NULL);

    token = strtok(line, " \t\n");
    while (token)
    {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;

    return (tokens);
}

char *get_path(char *cmd)
{
    char *path, *path_copy, *dir, *full_path;
    struct stat st;

    if (cmd[0] == '/' && stat(cmd, &st) == 0)
        return (strdup(cmd));

    path = getenv("PATH");
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    dir = strtok(path_copy, ":");

    while (dir)
    {
        full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);

        if (stat(full_path, &st) == 0)
        {
            free(path_copy);
            return (full_path);
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}

void print_error(char *cmd)
{
    write(STDERR_FILENO, "shell: 1: ", 10);
    write(STDERR_FILENO, cmd, strlen(cmd));
    write(STDERR_FILENO, ": not found\n", 12);
}

