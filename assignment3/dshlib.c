#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *command = NULL;       // for strtok_r
    char *saveCommand = NULL;   // for strtok_r
    char *token = NULL;         // for nested strtok_r
    char *saveToken = NULL;     // for nested strtok_r
    size_t tokenLength = 0;     // Size of each token
    size_t argsLength = 0;      // Counter for arguments
    int firstTime = 1;          // Boolean value for switching between exec and args

    clist->num = 0;             // Initialize clist->num to be zero

    // Split the commands by pipe
    command = strtok_r(cmd_line, PIPE_STRING, &saveCommand);
    while (command != NULL) 
    {
        // printf("%s\n", command);
        argsLength = 0;         // Reset the args length for command in each pipe

        ++clist->num;
        // printf("clist->num: %d\n", clist->nm);
        if ((clist->num) > CMD_MAX) 
        { 
            // printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }

        // exec command
        token = strtok_r(command, SPACE_STRING, &saveToken);
        

        // args command
        while (token != NULL) 
        {

            // Get the exec command first; if not go to arguments
            if (firstTime) 
            {
                tokenLength = strlen(token);
                if (tokenLength > EXE_MAX) { return ERR_CMD_OR_ARGS_TOO_BIG; }

                strcpy(clist->commands[clist->num - 1].exe, token);

                firstTime = 0;
            } 
            else 
            {

                tokenLength = strlen(token);

                if (argsLength + tokenLength > ARG_MAX) { return ERR_CMD_OR_ARGS_TOO_BIG; }
                for (size_t i = 0; i < tokenLength; i++) 
                {
                    clist->commands[clist->num - 1].args[argsLength] = token[i];
                    ++argsLength;
                }

                if (argsLength + 1 > ARG_MAX) { return ERR_CMD_OR_ARGS_TOO_BIG; }
                clist->commands[clist->num - 1].args[argsLength] = SPACE_CHAR;
                ++argsLength;
            }

            token = strtok_r(NULL, SPACE_STRING, &saveToken);
        }

        if (argsLength > 0) 
        {
            clist->commands[clist->num - 1].args[argsLength - 1] = '\0';
            // printf("%s\n", clist->commands[clist->num - 1].args);
        }

        command = strtok_r(NULL, PIPE_STRING, &saveCommand);
        firstTime = 1;
    }

    return OK;
}

// Print out the struct command_list_t
void print_cmd_list (command_list_t *clist) 
{
    printf(CMD_OK_HEADER, clist->num);
    
    for (int i = 1; i <= clist->num; i++) 
    {
        if (strlen(clist->commands[i-1].args) != 0) 
        {
            printf("<%d> %s [%s]\n", i, clist->commands[i-1].exe, clist->commands[i-1].args);
        } 
        else 
        {
            printf("<%d> %s\n", i, clist->commands[i-1].exe);
        }
    }
}

int encode_dragon_ascii (char *ascii_art, uint8_t *x) 
{
    int xCounter = 0;                       // Counter for each byte
    int i = 0;                              // Counter for loop through all characters in ascii representation
    char tmpChar;

    while (*(ascii_art + i) != '\0') 
    {
        uint8_t tmpInt = 0;

        // Loop through a byte
        for (int j = 0; j < 4; j++) 
        {

            // For last remaining byte
            if (*(ascii_art + i) == '\0') 
            { 
                // printf("Hello\n");
                tmpInt <<= 2;
                continue; 
            }

            tmpChar = *(ascii_art + i);

            // Left shift by 2 to make space
            tmpInt <<= 2; 

            if (tmpChar == ' ') { tmpInt |= SPACE; } 
            else if (tmpChar == '%') { tmpInt |= PERCENT; } 
            else if (tmpChar == '@') { tmpInt |= AT; } 
            else if (tmpChar == '\n') { tmpInt |= NEWLINE; }
    
            ++i;
        }

        x[xCounter] = tmpInt;
        ++xCounter;
    }

    return xCounter;
}

void decode_dragon_ascii (uint8_t *x, size_t length)
{
    for (size_t i = 0; i < length; i++) 
    {

        uint8_t tmpInt = x[i];
        
        for (int j = 6; j >= 0 ; j -= 2) 
        {
            uint8_t value = (tmpInt >> j) & 0b11;

            if (value == SPACE) { printf(" "); } 
            else if (value == PERCENT) { printf("%%"); } 
            else if (value == AT) { printf("@"); } 
            else if (value == NEWLINE) { printf("\n"); } 
            // else { printf("WHAT"); }
        }
    }

}