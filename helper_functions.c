#include "shell.h"

/**
 * _getenv - get environment variable
 * @name: variable name
 * Return: value or NULL
 */
char *_getenv(char *name)
{
	int i = 0;
	size_t len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 &&
		    environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_path - locate command in PATH
 * @command: command name
 * Return: full path or NULL
 */
char *find_path(char *command)
{
	char *path, *path_copy, *token, *full_path;
	struct stat st;

	if (command[0] == '/' && stat(command, &st) == 0)
		return (strdup(command));

	path = _getenv("PATH");
	if (!path)
		return (NULL);

	path_copy = strdup(path);
	token = strtok(path_copy, ":");

	while (token)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		sprintf(full_path, "%s/%s", token, command);

		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * split_line - split input into arguments
 * @line: input line
 * Return: array of strings
 */
char **split_line(char *line)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(sizeof(char *) * bufsize);
	char *token;

	token = strtok(line, " \t\n");
	while (token)
	{
		tokens[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	tokens[i] = NULL;

	return (tokens);
}

