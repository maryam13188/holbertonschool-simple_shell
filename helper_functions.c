#include "shell.h"

/* ===================== READ LINE ===================== */
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

/* ===================== TOKENIZE ===================== */
char **tokenize(char *line)
{
    int bufsize = 64, i = 0;
    char **tokens = malloc(sizeof(char *) * bufsize);
    char *token;

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

/* ===================== WHITESPACE CHECK ===================== */
int is_whitespace(char *line)
{
    int i = 0;

    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t')
            return (0);
        i++;
    }
    return (1);
}

/* ===================== FREE ARGS ===================== */
void free_args(char **args)
{
    free(args);
}

/* ===================== BUILTINS ===================== */
int handle_builtin(char **args)
{
    int i;

    if (strcmp(args[0], "exit") == 0)
        exit(0);

    if (strcmp(args[0], "env") == 0)
    {
        for (i = 0; environ[i]; i++)
        {
            write(STDOUT_FILENO, environ[i], strlen(environ[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        return (1);
    }
    return (0);
}

/* ===================== EXECUTE ===================== */
int execute(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, environ) == -1)
        {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        wait(&status);
    }

    return (1);
}

