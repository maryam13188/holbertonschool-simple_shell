#include "shell.h"

/**
 * shell_loop - Main shell loop
 */
void shell_loop(void)
{
    char *line = NULL;
    char **args = NULL;
    int last_status = 0;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        line = read_line();
        if (!line) /* EOF */
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            exit(last_status);
        }

        args = split_line(line);
        if (!args || !args[0])
        {
            free(line);
            free_tokens(args);
            continue;
        }

        /* Built-in exit */
        if (_strcmp(args[0], "exit") == 0)
        {
            free(line);
            free_tokens(args);
            exit(last_status);
        }

        last_status = execute(args);

        free(line);
        free_tokens(args);
    }
}

