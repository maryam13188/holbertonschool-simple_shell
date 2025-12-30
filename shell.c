#include "shell.h"

int execute_cmd(char **argv)
{
    pid_t pid;
    char *cmd_path;

    cmd_path = get_path(argv[0]);
    if (!cmd_path)
    {
        print_error(argv[0]);
        return (127);
    }

    pid = fork();
    if (pid == 0)
    {
        execve(cmd_path, argv, environ);
        exit(1);
    }
    else
        wait(NULL);

    free(cmd_path);
    return (0);
}

