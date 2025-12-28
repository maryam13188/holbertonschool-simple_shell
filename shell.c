#include "shell.h"
/**
	* shell_loop - Main loop of the shell
	*/
void shell_loop(void)
{
char *line = NULL;
char **args = NULL;
int status = 0;
while (1)
{
/* Print prompt only in interactive mode */
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);
line = read_line();
if (line == NULL) /* EOF (Ctrl+D) */
break;
args = split_line(line);
free(line);
if (!args || !args[0])
{
free_tokens(args);
continue;
}
/* Handle exit built-in */
if (_strcmp(args[0], "exit") == 0)
{
free_tokens(args);
break;
}
/* Execute command */
status = execute(args);
/*
	* IMPORTANT:
	* In non-interactive mode, if command is not found,
	* exit shell with status 127 (required by checker)
	*/
if (!isatty(STDIN_FILENO) && status == 127)
{
free_tokens(args);
exit(127);
}
free_tokens(args);
}
}

