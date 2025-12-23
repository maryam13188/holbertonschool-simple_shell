#include "shell.h"

/**
 * main - Entry point for simple shell
 * Return: Always 0
 */
int main(void)
{
    char *line;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        line = read_line();
        if (!line)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Check if line is only whitespace */
        if (is_whitespace(line))
        {
            free(line);
            continue;
        }

        args = split_line(line);
        if (!args || !args[0]) /* No commands */
        {
            free(line);
            free_tokens(args);
            continue;
        }

        execute(args);

        free(line);
        free_tokens(args);
    }

    return (0);
}
