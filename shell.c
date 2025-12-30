#include "shell.h"

/**
 * read_line - Reads a line from stdin
 * Return: The line read (without newline), NULL on EOF
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;

    printf(":) ");
    nread = getline(&line, &bufsize, stdin);

    if (nread == -1)
    {
        free(line);
        return (NULL);
    }

    /* Remove newline */
    if (line[nread - 1] == '\n')
        line[nread - 1] = '\0';

    return (line);
}

/**
 * parse_line - Parses a line into tokens
 * @line: Line to parse
 * Return: Array of tokens, NULL-terminated
 */
char **parse_line(char *line)
{
    return split_string(line, " \t\r\n\a");
}

/**
 * shell_loop - Main shell loop
 */
void shell_loop(void)
{
    char *line;
    char **args;
    int status = 1;

    while (status)
    {
        line = read_line();
        if (!line)
            break;

        args = parse_line(line);
        if (!args)
        {
            free(line);
            continue;
        }

        if (args[0])
        {
            if (is_builtin(args))
                handle_builtin(args);
            else
                handle_path_and_execute(args);
        }

        free_tokens(args);
        free(line);
    }

    printf("\n");
}
