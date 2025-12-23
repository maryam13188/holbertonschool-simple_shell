#include "shell.h"

/**
 * is_whitespace - Check if string contains only whitespace
 * @str: String to check
 * Return: 1 if only whitespace, 0 otherwise
 */
int is_whitespace(const char *str)
{
    int i;

    if (!str)
        return (1);

    for (i = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (0);
    }
    return (1);
}

/**
 * _strlen - Returns length of string
 * @s: String
 * Return: Length
 */
int _strlen(char *s)
{
    int len = 0;

    if (!s)
        return (0);

    while (s[len])
        len++;

    return (len);
}

/**
 * _strdup - Duplicates a string
 * @str: String to duplicate
 * Return: Duplicated string
 */
char *_strdup(char *str)
{
    char *dup;
    int i, len;

    if (!str)
        return (NULL);

    len = _strlen(str);
    dup = malloc(len + 1);

    if (!dup)
        return (NULL);

    for (i = 0; i <= len; i++)
        dup[i] = str[i];

    return (dup);
}

/**
 * free_tokens - Frees an array of tokens
 * @tokens: Array of tokens to free
 */
void free_tokens(char **tokens)
{
    int i = 0;

    if (!tokens)
        return;

    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }

    free(tokens);
}

/**
 * read_line - Reads a line from stdin
 * Return: Line read (must be freed) or NULL if EOF
 */
char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, stdin);
    if (read == -1)
    {
        free(line);
        return (NULL);
    }

    if (read > 0 && line[read - 1] == '\n')
        line[read - 1] = '\0';

    return (line);
}

/**
 * split_line - Split line into tokens
 * @line: Line to split
 * Return: Array of tokens
 */
char **split_line(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token != NULL)
    {
        /* Add token only if not empty */
        if (strlen(token) > 0)
        {
            tokens[position] = _strdup(token);
            if (!tokens[position])
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
            position++;
        }

        if (position >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }
    
    tokens[position] = NULL;
    return (tokens);
}
