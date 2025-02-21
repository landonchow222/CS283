// QtoTA:   How many bytes do i allocate for cmd_buff_t
//          How many bytes do i allocate for userinput

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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
    char *commandInput = (char *) malloc(ARG_MAX);
    if (commandInput == NULL) 
    {
        perror("!!! Malloc of commandInput Failed !!!");
        exit(ERR_MEMORY);
    }
    int rc = 0;
    int f_result, c_result;

    // Allocate cmd_buff_t
    cmd_buff_t *cmd_struct = NULL;
    alloc_cmd_buff(&cmd_struct);
    clear_cmd_buff(cmd_struct);
    


    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(commandInput, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        commandInput[strcspn(commandInput, "\n")] = '\0';

        // Maximum Argument Error Check
        // Take account for \n and \0
        if (strlen(commandInput) > ARG_MAX - 2) {
            
            // Deallocate cmd_buff_t
            free_cmd_buff(&cmd_struct);
            free(commandInput); commandInput = NULL;

            printf("!!! Maximum Input Character is 254 !!!\n");
            exit(ERR_CMD_OR_ARGS_TOO_BIG);
        }


        rc = build_cmd_buff(commandInput, cmd_struct);
        if (rc == WARN_NO_CMDS) 
        {   
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        // printf("Argc: %d\n", cmd_struct->argc);
        // size_t j = 0;
        // while (cmd_struct->argv[j]) {
        //     printf("%s\n", cmd_struct->argv[j]);
        //     ++j;
        // }


        if (strcmp(cmd_struct->argv[0], EXIT_CMD) == 0)   // exit command
        {
            rc = OK;
            break;
        }

        else if (strcmp(cmd_struct->argv[0], DRAGON_CMD) == 0) // dragon command
        {
            // Uncompressed Print
            // printf("%s", DRAGON_ASCII_ART);

            // Compressed Print
            print_dragon();


        }

        else if (strcmp(cmd_struct->argv[0], CD_CMD) == 0) 
        {
            char *dirLoc = (char *) malloc(100);
            if (dirLoc == NULL) 
            {
                perror("!!! Malloc of dirLoc Failed !!!");
                exit(ERR_MEMORY);
            }

            if (chdir(cmd_struct->argv[1]) == -1)
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

        else 
        {
            f_result = fork();
            if (f_result < 0) {
                perror("fork error");
                exit(1);
            }

            if (f_result == 0) 
            {
                rc = execvp(cmd_struct->argv[0], cmd_struct->argv);
                // Make sure you include the exid command below in case of execution failure
                exit(ERR_EXEC_CMD);
            } 
            else 
            {
                wait(&c_result);
            }
        }

        clear_cmd_buff(cmd_struct);
    }


    // Deallocate cmd_buff_t
    free_cmd_buff(&cmd_struct);
    free(commandInput); commandInput = NULL;

    return OK;
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

int free_cmd_buff(cmd_buff_t **cmd_buff)
{
    free(*cmd_buff);
    *cmd_buff = NULL;

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

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = cmd_line;

    if (strlen(cmd_buff->_cmd_buffer) == 0) {
        return WARN_NO_CMDS;
    }

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


// Built_In_Cmds match_command(const char *input) 
// {
//     if (strcmp(input, "exit") == 0) 
//     {
//         return BI_CMD_EXIT;
//     }
//     else if (strcmp(input, "cd") == 0)
//     {
//         return BI_CMD_CD;
//     }
//     else if (strcmp(input, "dragon") == 0) 
//     {
//         return BI_CMD_DRAGON;
//     }
// }

// Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
// {
//     return;
// }
