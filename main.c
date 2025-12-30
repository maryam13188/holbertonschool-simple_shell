#include "shell.h"

int main(void)
{
    char *line;
    char **argv;

    while (1)
    {
        write(STDOUT_FILENO, ":) ", 3);

        line = read_line();
        if (!line)
            break;

        argv = tokenize(line);
        if (!argv || !argv[0])
        {
            free(line);
            free(argv);
            continue;
        }

        execute_cmd(argv);

        free(line);
        free(argv);
    }
    return (0);
}

