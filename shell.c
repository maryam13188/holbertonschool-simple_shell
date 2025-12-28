#include "shell.h"
void shell_loop(void)
{
char *line = NULL;
char **args = NULL;
while (1)
{
/* Print prompt ONLY if interactive */
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "$ ", 2);
line = read_line();
if (!line) /* EOF (Ctrl+D) */
break;
args = split_line(line);
free(line);
if (!args || !args[0])
{
free_tokens(args);
continue;
}
if (_strcmp(args[0], "exit") == 0)
{
free_tokens(args);
break;
}
execute(args);
free_tokens(args);
}
}
