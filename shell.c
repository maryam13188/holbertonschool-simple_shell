#include "shell.h"

void shell_loop(void)
{
    char *line, **args;
    int last_status = 0;

    while (1)
    {
        line = read_line();
        if (!line) break;

        args = split_line(line);
        if (!args || !args[0])
        {
            free(line);
            free_tokens(args);
            continue;
        }

        if (_strcmp(args[0], "exit") == 0)
        {
            free(line);
            free_tokens(args);
            exit(last_status);
        }

        last_status = execute_external(args);
        
        free(line);
        free_tokens(args);
    }
}
