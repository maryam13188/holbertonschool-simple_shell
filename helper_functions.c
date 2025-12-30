#include "shell.h"
#include <sys/stat.h>

/* ================= PATH Helpers ================= */

char *get_path_from_environ(void)
{
    extern char **environ;
    int i = 0;
    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return (environ[i] + 5);
        i++;
    }
    return NULL;
}

int check_command(char *command)
{
    struct stat st;
    char *path = get_path_from_environ();
    char *path_copy, *dir, *full_path;

    if (!command || command[0] == '\0')
        return 0;

    if (strchr(command, '/'))
        return (stat(command, &st) == 0 && S_ISREG(st.st_mode));

    if (!path)
        return 0;

    path_copy = _strdup(path);
    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        sprintf(full_path, "%s/%s", dir, command);
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
        {
            free(full_path);
            free(path_copy);
            return 1;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return 0;
}

char *find_full_path(char *command)
{
    struct stat st;
    char *path = get_path_from_environ();
    char *path_copy, *dir, *full_path;

    if (!command || command[0] == '\0')
        return NULL;

    if (strchr(command, '/'))
        return (stat(command, &st) == 0 && S_ISREG(st.st_mode)) ? _strdup(command) : NULL;

    if (!path)
        return NULL;

    path_copy = _strdup(path);
    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        sprintf(full_path, "%s/%s", dir, command);
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

/* ================= Execute ================= */

int execute(char **args)
{
    pid_t pid;
    int status;
    char *full_path;

    if (!args || !args[0])
        return 1;

    if (!check_command(args[0]))
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return 127;
    }

    full_path = find_full_path(args[0]);
    if (!full_path)
        return 127;

    pid = fork();
    if (pid == 0) /* Child */
    {
        if (execve(full_path, args, environ) == -1)
        {
            perror("execve");
            free(full_path);
            exit(126);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
        free(full_path);
        return 1;
    }
    else /* Parent */
    {
        waitpid(pid, &status, 0);
        free(full_path);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    return 1;
}

