#include "shell.h"

/* String Functions */
int is_whitespace(const char *str)
{
    int i; if (!str) return (1);
    for (i = 0; str[i]; i++)
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (0);
    return (1);
}

char *_strdup(const char *str)
{
    char *dup; int i, len = 0;
    if (!str) return (NULL);
    while (str[len]) len++;
    dup = malloc(len + 1);
    if (!dup) return (NULL);
    for (i = 0; i <= len; i++) dup[i] = str[i];
    return (dup);
}

int _strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i]) i++;
    return (s1[i] - s2[i]);
}

void free_args(char **args)
{
    int i = 0; if (!args) return;
    while (args[i]) free(args[i++]);
    free(args);
}

/* Input/Output */
char *read_line(void)
{
    char *line = NULL; size_t len = 0;
    ssize_t read = getline(&line, &len, stdin);
    if (read == -1) { free(line); return (NULL); }
    if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
    return (line);
}

char **split_line(char *line)
{
    int bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    if (!tokens) { fprintf(stderr, "Allocation error\n"); exit(EXIT_FAILURE); }
    token = strtok(line, " \t\r\n");
    while (token)
    {
        tokens[pos] = _strdup(token);
        if (!tokens[pos]) { fprintf(stderr, "Allocation error\n"); exit(EXIT_FAILURE); }
        pos++;
        if (pos >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) { fprintf(stderr, "Allocation error\n"); exit(EXIT_FAILURE); }
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[pos] = NULL;
    return (tokens);
}

/* Built-in Commands */
void print_environment(void)
{
    int i = 0;
    while (environ[i]) printf("%s\n", environ[i++]);
}

int handle_builtin(char **args)
{
    if (!args || !args[0]) return (0);
    if (_strcmp(args[0], "exit") == 0) exit(0);
    if (_strcmp(args[0], "env") == 0) { print_environment(); return (1); }
    return (0);
}

/* PATH Functions */
char *find_command_in_path(char *command)
{
    char *path_env, *path_copy, *dir, *full_path;
    struct stat st;
    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0 && S_ISREG(st.st_mode))
            return (_strdup(command));
        return (NULL);
    }
    path_env = getenv("PATH");
    if (!path_env || path_env[0] == '\0') return (NULL);
    path_copy = _strdup(path_env);
    if (!path_copy) return (NULL);
    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        if (!full_path) { free(path_copy); return (NULL); }
        sprintf(full_path, "%s/%s", dir, command);
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
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

int command_exists(char *command)
{
    char *full_path = find_command_in_path(command);
    if (full_path) { free(full_path); return (1); }
    return (0);
}

/* Execution */
int execute(char **args)
{
    pid_t pid;
    int status;
    char *full_path;
    if (!args || !args[0]) return (1);
    if (!command_exists(args[0]))
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return (1);
    }
    full_path = find_command_in_path(args[0]);
    if (!full_path)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return (1);
    }
    pid = fork();
    if (pid == 0)
    {
        if (execve(full_path, args, environ) == -1)
        {
            fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
        free(full_path);
        return (1);
    }
    else
    {
        waitpid(pid, &status, 0);
        free(full_path);
    }
    return (1);
}
