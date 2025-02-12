#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char *cmd_buff = (char *) malloc(5000);     // Input Buffer
    if (!cmd_buff) {
        printf(MALLOC_FAILURE);
        return ERR_ALLOCATION_FAILED;
    }
    int rc = 0;                                 // Return Values
    command_list_t clist;                       // Command Struct
    uint8_t encodedAscii[DRAGON_ASCII_ART_LEN]; // Dragon Logo Encoded Array

    command_list_t *memsetReturn;               // Return Value for memset()

    

    while (1)
    {
        // Set all the values inside the struct to zero
        memsetReturn = memset(&clist, 0, sizeof(clist));
        if (memsetReturn == NULL){ printf("Something went Wrong!"); }
        // Set all the values inside the encoded dragon representation to zero
        memsetReturn = memset(encodedAscii, 0, sizeof(encodedAscii));
        if (memsetReturn == NULL){ printf("Something went Wrong!"); }



        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        // printf("%s\n", cmd_buff);



        if (strncmp(cmd_buff, EXIT_CMD, EXIT_CMD_BYTES) == 0)   // exit command
        {
            rc = 0;
            break;
        } 
        else if (strncmp(cmd_buff, DRAGON_CMD, DRAGON_CMD_BYTES) == 0) // dragon command
        {
            // Uncompressed Print
            // printf("%s", DRAGON_ASCII_ART);

            // Compressed Print
            encode_dragon_ascii(DRAGON_ASCII_ART, encodedAscii);
            decode_dragon_ascii(encodedAscii, DRAGON_ASCII_ART_LEN);
        }
        else if (cmd_buff[0] == '\0')  // no command is typed
        {
            rc = WARN_NO_CMDS;
            printf(CMD_WARN_NO_CMD);
        } 
        else 
        {
            
            // Import the input into "command_list_t" struct
            rc = build_cmd_list(cmd_buff, &clist);

            if (rc == ERR_TOO_MANY_COMMANDS) 
            {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            } 
            else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) 
            {
                // return ERR_CMD_OR_ARGS_TOO_BIG;
            } 
            else if (rc == 0) 
            {    
                print_cmd_list(&clist);
            }
        }
    }

    free(cmd_buff);
    cmd_buff = NULL;

    return rc;    
}

