#include "shell.h"

/* String utilities */
char *_strdup(char *str)
{
    char *dup;
    if (!str) return NULL;
    dup = malloc(strlen(str) + 1);
    if (!dup) return NULL;
    strcpy(dup, str);
    return dup;
}

int _strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;
    return *s1 - *s2;
}

void print_error(char *cmd)
{
    if (!cmd) return;
    write(2, cmd, strlen(cmd));
    write(2, ": not found\n", 12);
}

/* Input functions */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;
    
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "$ ", 2);
    
    nread = getline(&line, &bufsize, stdin);
    
    if (nread == -1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "\n", 1);
        free(line);
        return NULL;
    }
    
    if (nread > 0 && line[nread - 1] == '\n')
        line[nread - 1] = '\0';
    
    return line;
}

char **split_line(char *line)
{
    int bufsize = 64, i = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    
    if (!tokens) return NULL;
    
    token = strtok(line, " \t\r\n");
    while (token)
    {
        if (strlen(token) > 0)
        {
            tokens[i] = _strdup(token);
            if (!tokens[i])
            {
                free_tokens(tokens);
                return NULL;
            }
            i++;
        }
        
        if (i >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) return NULL;
        }
        
        token = strtok(NULL, " \t\r\n");
    }
    tokens[i] = NULL;
    return tokens;
}

void free_tokens(char **tokens)
{
    int i = 0;
    
    if (!tokens) return;
    
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    
    free(tokens);
}

/* Command execution */
int execute_external(char **args)
{
    pid_t pid;
    int status;
    struct stat st;

    if (!args || !args[0])
        return (1);

    /* Check if command exists */
    if (stat(args[0], &st) == -1)
    {
        print_error(args[0]);
        return (127);
    }

    /* Check if it's a regular file and executable */
    if (!S_ISREG(st.st_mode))
    {
        print_error(args[0]);
        return (126);
    }

    if (!(st.st_mode & S_IXUSR))
    {
        print_error(args[0]);
        return (126);
    }

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execve(args[0], args, environ) == -1)
        {
            perror(args[0]);
            exit(127);
        }
    }
    else if (pid < 0)
    {
        /* Fork failed */
        perror("fork");
        return (1);
    }
    else
    {
        /* Parent process */
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else
            return (1);
    }
    
    return (1);
}
