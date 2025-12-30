#include "shell.h"

int shell_loop(void)  
{
    char *line, **args;
    int last_status = 0;
    int line_count = 1;

    while (1)
    {
        line = read_line();
        if (!line) 
        {
            
            return last_status;  
        }

        args = split_line(line);
        if (!args || !args[0])
        {
            free(line);
            free_tokens(args);
            line_count++;
            continue;
        }

        if (_strcmp(args[0], "exit") == 0)
        {
            free(line);
            free_tokens(args);
            exit(last_status);  
        }

        last_status = execute_command(args, line_count);
        
        free(line);
        free_tokens(args);
        line_count++;
    }
    
    return last_status;  
}
