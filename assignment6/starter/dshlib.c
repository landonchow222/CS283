#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *commandInput = (char *) malloc(ARG_MAX);          // Main Input From User
    if (commandInput == NULL) 
    {
        perror("!!! Malloc of commandInput Failed !!!");
        exit(ERR_MEMORY);
    }
    int rc = 0;
    int wexit_result = -1;

    // // Allocate cmd_buff_t
    // cmd_buff_t *cmd_struct = NULL;
    // alloc_cmd_buff(&cmd_struct);
    // clear_cmd_buff(cmd_struct);
    
    // Allocate command_list_t
    command_list_t *cmd_list = NULL;
    alloc_cmd_list(&cmd_list);

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(commandInput, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        commandInput[strcspn(commandInput, "\n")] = '\0';

        // // Maximum Argument Error Check
        // // Take account for \n and \0
        // if (strlen(commandInput) > ARG_MAX - 2) {
            
        //     // Deallocate cmd_buff_t
        //     free_cmd_buff(&cmd_struct);
        //     free(commandInput); commandInput = NULL;

        //     printf("!!! Maximum Input Character is 254 !!!\n");
        //     exit(ERR_CMD_OR_ARGS_TOO_BIG);
        // }

        rc = build_cmd_list(commandInput, cmd_list);
        // // Debugging Purpose
        // printf("Outside Build Cmd List:%s\n", commandInput);
        // printf("num:%d\n", cmd_list->num);
        // for (int j = 0; j < cmd_list->num; j++)
        // {
        //     for (int i = 0; i < cmd_list->commands[j].argc - 1; i++)
        //     {
        //         printf("Args:%s\n", cmd_list->commands[j].argv[i]);
        //     }

        //     printf("%s\n", cmd_list->commands[j]._cmd_buffer);
        // }
        
        // rc = build_cmd_buff(commandInput, cmd_struct);

        if (rc == WARN_NO_CMDS) 
        {   
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }
        // Debugging Purpose
        // printf("Argc: %d\n", cmd_struct->argc);
        // size_t j = 0;
        // while (cmd_struct->argv[j]) {
        //     printf("%s\n", cmd_struct->argv[j]);
        //     ++j;
        // }
        // printf("%s\n", cmd_struct->_cmd_buffer);


        if (cmd_list->num == 1)
        {
            if (strcmp(cmd_list->commands[0].argv[0], EXIT_CMD) == 0)   // exit command
            {
                rc = OK;
                break;
            }

            else if (strcmp(cmd_list->commands[0].argv[0], DRAGON_CMD) == 0) // dragon command
            {
                // Uncompressed Print
                // printf("%s", DRAGON_ASCII_ART);

                // Compressed Print
                print_dragon();
            }

            else if (strcmp(cmd_list->commands[0].argv[0], CD_CMD) == 0) 
            {
                char *dirLoc = (char *) malloc(100);
                if (dirLoc == NULL) 
                {
                    perror("!!! Malloc of dirLoc Failed !!!");
                    exit(ERR_MEMORY);
                }

                if (chdir(cmd_list->commands[0].argv[1]) == -1)
                {
                    // perror("!!! chdir Error !!!");
                    // free(dirLoc); dirLoc = NULL;
                    // exit(ERR_EXEC_CMD);
                } 
                else 
                {
                    getcwd(dirLoc, 100);
                    // printf("%s\n", dirLoc);
                }

                free(dirLoc); dirLoc = NULL;
            }

            else if (strcmp(cmd_list->commands[0].argv[0], RC_CMD) == 0)
            {
                if (wexit_result != -1) { printf("%d\n", wexit_result); }
            }

            else 
            {
                rc = execute_pipeline(cmd_list);
            }
            
        }
        else
        {
            rc = execute_pipeline(cmd_list);
        }

        clear_cmd_list(cmd_list);
    }


    // // Deallocate cmd_buff_t
    // free_cmd_buff(&cmd_struct);
    free(commandInput); commandInput = NULL;
    free_cmd_list(&cmd_list);

    return rc;
}





int alloc_cmd_buff(cmd_buff_t **cmd_buff)
{
    *cmd_buff = (cmd_buff_t *) malloc(1000);
    if (!(*cmd_buff)) 
    {
        perror("!!!cmd_buff_t malloc allocation failed!!!\n");
        exit(ERR_MEMORY);
    }

    return OK;
}

int alloc_cmd_list(command_list_t **cmd_list)
{
    *cmd_list = (command_list_t *) malloc(8000);
    if (!(*cmd_list)) 
    {
        perror("!!!command_list_t malloc allocation failed!!!\n");
        exit(ERR_MEMORY);
    }

    return OK;
}

int free_cmd_buff(cmd_buff_t **cmd_buff)
{
    free(*cmd_buff);
    *cmd_buff = NULL;

    return OK;
}

int free_cmd_list(command_list_t **cmd_list)
{
    free(*cmd_list);
    *cmd_list = NULL;

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff_t *memsetReturn = memset(cmd_buff, 0 , sizeof(*cmd_buff));
    if (!memsetReturn)
    {
        perror("!!!Clearing cmd_buff_t failed!!!\n");
        exit(ERR_MEMORY);
    }

    return OK;
}

int clear_cmd_list(command_list_t *cmd_list) 
{
    command_list_t *memsetReturn = memset(cmd_list, 0, sizeof(*cmd_list));
    if (!memsetReturn)
    {
        perror("!!!Clearing command_list_t failed!!!\n");
        exit(ERR_MEMORY);
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = cmd_line;

    if (strlen(cmd_buff->_cmd_buffer) == 0) { return WARN_NO_CMDS; }

    cmd_buff->argc = 0;
    size_t i = 0;

    bool in_quote_mode = false;
    bool is_word = false;
    
    while (cmd_buff->_cmd_buffer[i])
        {
            char c = cmd_buff->_cmd_buffer[i];
            // printf("%c", c);

            // c == '"'
            if (c == 34) 
            {
                if (in_quote_mode) 
                {
                    in_quote_mode = false;
                    cmd_buff->_cmd_buffer[i] = '\0';

                    // Check whether we are at end of word
                    if (is_word) { is_word = false; }
                } 
                else 
                {
                    in_quote_mode = true;
                }
            }

            // c == ' '
            if (c == 32)
            {
                if (!in_quote_mode)
                {
                    // Check whether we are at end of word
                    if (is_word) { is_word = false; }

                    cmd_buff->_cmd_buffer[i] = '\0';
                } 
                else if (in_quote_mode)
                {
                    if (!is_word) 
                    {
                        is_word = true;
                        cmd_buff->argv[cmd_buff->argc] = &cmd_buff->_cmd_buffer[i];
                        ++(cmd_buff->argc);
                    }
                }
                

            } else if (c >= 33 && c <= 126 && c != 34) {
                if (!is_word) 
                {
                    is_word = true;
                    cmd_buff->argv[cmd_buff->argc] = &cmd_buff->_cmd_buffer[i];
                    ++(cmd_buff->argc);
                }

            }
 
            ++i;

            // Argument Counter Error Check
            if (cmd_buff->argc > 8) 
            {
                printf(CMD_ERR_PIPE_LIMIT, 8);
                exit(ERR_TOO_MANY_COMMANDS);
            }
        }

        // Point to '\0' of input (BE CAREFUL HERE)
        // cmd_buff->argv[cmd_buff->argc] = cmd_buff->_cmd_buffer[i];
        cmd_buff->argv[cmd_buff->argc] = '\0';
        ++(cmd_buff->argc);

        // Execution Command Maximum Character Error Check
        if (strlen(cmd_buff->argv[0]) > EXE_MAX) 
        {
            printf("!!! Max Character Limit for Exec Command is 64 !!!\n");
            exit(ERR_CMD_OR_ARGS_TOO_BIG);
        }

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) 
{
    int rc;                         // Return Value

    char* command = NULL;           // For strtok_r (Splitting Commands)
    char* saveCommand = NULL;       // For strtok_r (Splitting Commands)
    
    clist->num = 0;
    
    cmd_buff_t *cmd_struct = NULL;
    alloc_cmd_buff(&cmd_struct);
    clear_cmd_buff(cmd_struct);

    command = strtok_r(cmd_line, PIPE_STRING, &saveCommand);
    while (command != NULL)
    {
        // printf("Command: %s\n", command);
        // printf("SaveCommand: %s\n", saveCommand);

        rc = build_cmd_buff(command, cmd_struct);
        if (rc == WARN_NO_CMDS) { return WARN_NO_CMDS; }
        // // Debugging Purpose
        // printf("Argc: %d\n", cmd_struct->argc);
        // size_t j = 0;
        // while (cmd_struct->argv[j]) {
        //     printf("%s\n", cmd_struct->argv[j]);
        //     ++j;
        // }

        // Check maximum commands
        if (clist->num == CMD_MAX)
        {   
            free_cmd_buff(&cmd_struct);
            return ERR_TOO_MANY_COMMANDS;
        }

        clist->commands[clist->num].argc = cmd_struct->argc; 
        for (int i = 0; i < cmd_struct->argc - 1; i++)
        {
            clist->commands[clist->num].argv[i] = strdup(cmd_struct->argv[i]); 
            // printf("%s\n", clist->commands[clist->num].argv[i]);
        }
        clist->commands[clist->num].argv[cmd_struct->argc - 1] = '\0'; 
        // printf("%s\n", cmd_struct->_cmd_buffer);
        clist->commands[clist->num]._cmd_buffer = strdup(cmd_struct->_cmd_buffer);
        // printf("%s\n", clist->commands[clist->num]._cmd_buffer);
        ++clist->num;
        
        clear_cmd_buff(cmd_struct);
        command = strtok_r(NULL, PIPE_STRING, &saveCommand);
    }

    // Deallocate cmd_buff_t
    free_cmd_buff(&cmd_struct);

    return OK;
}

// void execute_pipeline(Command commands[], int num_commands)
int execute_pipeline(command_list_t *cmd_list)
{
    int c_result;                       // Error number
    int wexit_result;       

    int num_commands = cmd_list->num;   // Number of Commands

    int pipes[num_commands - 1][2];     // Array of pipes
    pid_t pids[num_commands];           // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++) 
    {
        if (pipe(pipes[i]) == -1) 
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) 
    {
        pids[i] = fork();
        if (pids[i] == -1) 
        {
            perror("Fork Error");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process

            // Extra Credit for '>', '<', '>>'
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

            // >
            for (int j = 0; j < cmd_list->commands[i].argc - 1; j++) 
            {
                // printf("><:%s\n", cmd_list->commands[i].argv[j]);
                if (strcmp(cmd_list->commands[i].argv[j], ">") == 0)
                {
                    // printf("In:%s\n", cmd_list->commands[i].argv[j]);
                    // printf("In:%s\n", cmd_list->commands[i].argv[j+1]);

                    int fd = open(cmd_list->commands[i].argv[j+1], O_WRONLY | O_CREAT | O_TRUNC, mode);
                    if (fd < 0)
                    {
                        printf("!!!Error Opening the File!!!");
                        exit(ERR_OPEN_FILE);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);

                    cmd_list->commands[i].argv[j] = '\0';
                    break;
                } 
                
                else if (strcmp(cmd_list->commands[i].argv[j], "<") == 0)
                {
                    int fd = open(cmd_list->commands[i].argv[j+1], O_RDONLY, mode);
                    if (fd < 0)
                    {
                        printf("!!!Error Opening the File!!!");
                        exit(ERR_OPEN_FILE);
                    }

                    dup2(fd, STDIN_FILENO);
                    close(fd);

                    cmd_list->commands[i].argv[j] = '\0';
                    break;
                }

                else if (strcmp(cmd_list->commands[i].argv[j], ">>") == 0)
                {
                    int fd = open(cmd_list->commands[i].argv[j+1], O_WRONLY | O_CREAT | O_APPEND, mode);
                    if (fd < 0)
                    {
                        printf("!!!Error Opening the File!!!");
                        exit(ERR_OPEN_FILE);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);

                    cmd_list->commands[i].argv[j] = '\0';
                    break;
                }
            }

            // Set up input pipe for all except first process
            if (i > 0) { dup2(pipes[i-1][0], STDIN_FILENO); }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) { dup2(pipes[i][1], STDOUT_FILENO); }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) 
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            // execvp(commands[i].args[0], commands[i].args);
            execvp(cmd_list->commands[i].argv[0], cmd_list->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) 
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) 
    {
        waitpid(pids[i], &c_result, 0);
        // printf("Num: %d\n", c_result);
        wexit_result = getErrorNumber(c_result);
        // return wexit_result;
    }

    return wexit_result;
}

int getErrorNumber (int c_result) {
    int wexit_result = -1;

    switch (WEXITSTATUS(c_result)) 
    {
        case ENOENT:    // File Not Found
            wexit_result = ENOENT;
            printf("!!! File Not Found !!!\n");
            break;
        
        case EACCES:    // Permission Denied
            wexit_result = EACCES;
            printf("!!! Permission Denied (Command Not Found In Path) !!!\n");
            break;

        case EBADF:     // Bad File Number
            wexit_result = EBADF;
            printf("!!! Bad File Number !!!\n");
            break;
        
        case EEXIST:    // File Exists
            wexit_result = EEXIST;
            printf("!!! File Exists !!!\n");
            break;
        
        case ENFILE:    // File Table Overflow
            wexit_result = ENFILE;
            printf("!!! File Table Overflow !!!\n");
            break;

        case EMFILE:    // Too many open files
            wexit_result = EMFILE;
            printf("!!! Too Many Open Files !!!\n");
            break;

        case EFBIG:     // File Too Large
            wexit_result = EFBIG;
            printf("!!! File Too Large !!!\n");
            break;

        case EROFS:     // Read only file system
            wexit_result = EROFS;
            printf("!!! Read Only File System !!!\n");
            break;

        case EBADFD:    // File Descriptor in Bad Shape
            wexit_result = EBADFD;
            printf("!!! File Descriptor in Bad Shape !!!\n");
            break;

        case EINVAL:    // Invalid Argument
            wexit_result = EINVAL;
            printf("!!! Invalid Argument !!!\n");
            break;

        case E2BIG:     // Argument List Too Long
            wexit_result = E2BIG;
            printf("!!! Argument List Too Long !!!\n");
            break;

        case ENOEXEC:   // Execution Format Error
            wexit_result = ENOEXEC;
            printf("!!! Execution Format Error !!!\n");
            break;

        case EPERM: // Operation not permitted
            wexit_result = ENOEXEC;
            printf("!!! Operation Not Permitted !!!\n");
            break;

        // // Child Process Failure meaning
        // // Command does not exist
        // // This is not from errno.h
        // case ERR_EXEC_CMD + 256:
        //     wexit_result = ERR_EXEC_CMD + 256;
        //     printf("!!! Command Not Found In PATH !!!\n");
        //     break;

        default:
            wexit_result = WEXITSTATUS(c_result);

    }

    return wexit_result;
}

