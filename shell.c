#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
	* shell_loop - Main shell loop
	*/
void shell_loop(void)
{
char *line;
char **args;
int status = 1;
do
{
write(1, "$ ", 2);
line = read_line();
if (!line)  /* EOF */
{
write(1, "\n", 1);
break;
}
args = split_line(line);
free(line);
if (!args)
continue;
/* Exit command */
if (_strcmp(args[0], "exit") == 0)
{
free_tokens(args);
break;
}
status = execute(args);
free_tokens(args);
} while (status);
}
