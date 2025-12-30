#include "shell.h"

/**
 * shell_loop - main shell loop
 */
void shell_loop(void)
{
	char *line = NULL, *cmd_path;
	size_t len = 0;
	ssize_t read;
	char **args;
	pid_t pid;
	int status;

	while (1)
	{
		write(STDOUT_FILENO, "$ ", 2);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		args = split_line(line);
		if (!args[0])
		{
			free(args);
			continue;
		}

		cmd_path = find_path(args[0]);
		if (!cmd_path)
		{
			write(STDERR_FILENO, args[0], strlen(args[0]));
			write(STDERR_FILENO, ": command not found\n", 20);
			free(args);
			continue; /* ❗ no fork */
		}

		pid = fork();
		if (pid == 0)
		{
			execve(cmd_path, args, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
			wait(&status);

		free(cmd_path);
		free(args);
	}
	free(line);
}

/**
 * main - entry point
 * Return: 0
 */
int main(void)
{
	shell_loop();
	return (0);
}

