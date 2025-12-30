#include "shell.h"

int main(void)
{
    char *line, **args;
    int status = 1;
    
    while (status)
    {
        if (isatty(STDIN_FILENO)) printf(":) ");
        line = read_line();
        if (!line)
        {
            if (isatty(STDIN_FILENO)) printf("\n");
            break;
        }
        if (is_whitespace(line)) { free(line); continue; }
        args = split_line(line);
        if (!args || !args[0]) { free(line); if (args) free_args(args); continue; }
        if (handle_builtin(args)) { free(line); free_args(args); continue; }
        status = execute(args);
        free(line);
        free_args(args);
    }
    return (0);
}
