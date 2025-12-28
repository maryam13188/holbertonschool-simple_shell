#include "shell.h"
/**
	* shell_loop - main shell loop
	*/
void shell_loop(void)
{
char *line = NULL;
char **args = NULL;
int status = 1;
while (status)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);
line = read_line();
if (line == NULL)
{
/* Ctrl+D */
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
break;
}
args = split_line(line);
if (args == NULL || args[0] == NULL)
{
free(line);
free_tokens(args);
continue;
}
/* 🔹 built-in: exit */
if (strcmp(args[0], "exit") == 0)
{
free(line);
free_tokens(args);
exit(0);
}
status = execute(args);
free(line);
free_tokens(args);
}
}
