#include "shell.h"

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
 * Return: Line read (must be freed)
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

	if (line[read - 1] == '\n')
		line[read - 1] = '\0';

	return (line);
}

/**
 * split_line - Splits a line into tokens
 * @line: Line to split
 * Return: Array of tokens
 */
char **split_line(char *line)
{
	int bufsize = BUFFER_SIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
		return (NULL);

	token = strtok(line, TOKEN_DELIM);
	while (token != NULL)
	{
		tokens[position] = _strdup(token);
		if (!tokens[position])
		{
			free_tokens(tokens);
			return (NULL);
		}
		position++;

		if (position >= bufsize)
		{
			bufsize += BUFFER_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				free_tokens(tokens);
				return (NULL);
			}
		}

		token = strtok(NULL, TOKEN_DELIM);
	}

	tokens[position] = NULL;
	return (tokens);
}

/**
 * execute - Executes a command
 * @args: Array of arguments
 * Return: 1 to continue, 0 to exit
 */
int execute(char **args)
{
	pid_t pid;
	int status;
	struct stat st;

	if (stat(args[0], &st) == -1)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		return (1);
	}

	if (!(st.st_mode & S_IXUSR))
	{
		fprintf(stderr, "./hsh: 1: %s: Permission denied\n", args[0]);
		return (1);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(args[0], args, NULL) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else
	{
		waitpid(pid, &status, 0);
	}

	return (1);
}
