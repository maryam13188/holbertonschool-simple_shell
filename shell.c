#include "shell.h"
/**
	* get_path_from_environ - Get PATH from environ directly
	* Return: PATH string or NULL
	*/
char *get_path_from_environ(void)
{
extern char **environ;
int i = 0;
char *path_prefix = "PATH=";
int prefix_len = 5;
if (!environ)
return (NULL);
while (environ[i])
{
if (strncmp(environ[i], path_prefix, prefix_len) == 0)
return (environ[i] + prefix_len);
i++;
}
return (NULL);
}
/**
	* check_command - Check if command exists
	* @command: Command to check
	* Return: 1 if exists, 0 if not
	*/
int check_command(char *command)
{
struct stat st;
char *path_copy, *dir, *full_path;
char *path = get_path_from_environ();
/* Check if command is NULL or empty */
if (!command || command[0] == '\0')
return (0);
/* If command contains '/', check it directly */
if (strchr(command, '/') != NULL)
{
if (stat(command, &st) == 0 && S_ISREG(st.st_mode))
return (1);
return (0);
}
if (!path)
return (0);
path_copy = _strdup(path);
if (!path_copy)
return (0);
dir = strtok(path_copy, ":");
while (dir != NULL)
{
full_path = malloc(strlen(dir) + strlen(command) + 2);
if (!full_path)
{
free(path_copy);
return (0);
}
sprintf(full_path, "%s/%s", dir, command);
if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
{
free(full_path);
free(path_copy);
return (1);
}
free(full_path);
dir = strtok(NULL, ":");
}
free(path_copy);
return (0);
}
/**
	* find_full_path - Find full path of command
	* @command: Command to find
	* Return: Full path or NULL
	*/
char *find_full_path(char *command)
{
struct stat st;
char *path_copy, *dir, *full_path;
char *path = get_path_from_environ();
/* Check if command is NULL or empty */
if (!command || command[0] == '\0')
return (NULL);
/* If command contains '/', return it as is if exists */
if (strchr(command, '/') != NULL)
{
if (stat(command, &st) == 0 && S_ISREG(st.st_mode))
return (_strdup(command));
return (NULL);
}
if (!path)
return (NULL);
path_copy = _strdup(path);
if (!path_copy)
return (NULL);
dir = strtok(path_copy, ":");
while (dir != NULL)
{
full_path = malloc(strlen(dir) + strlen(command) + 2);
if (!full_path)
{
free(path_copy);
return (NULL);
}
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
/**
	* execute - Execute a command
	* @args: Array of command and arguments
	* Return: 1 to continue, 0 to exit
	*/
int execute(char **args)
{
pid_t pid;
int status;
char *full_path; 
if (!args || !args[0] || args[0][0] == '\0')
return (1);
/* Check if command exists before forking */
if (!check_command(args[0]))
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return (1);
}
/* Find the full path */
full_path = find_full_path(args[0]);
if (!full_path)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return (1);
}
pid = fork();
if (pid == 0) /* Child process */
{
if (execve(full_path, args, environ) == -1)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
free(full_path);
exit(EXIT_FAILURE);
}
}
else if (pid < 0) /* Fork error */
{
perror("fork");
free(full_path);
return (1);
}
else /* Parent process */
{
waitpid(pid, &status, 0);
free(full_path);
}
return (1);
}
