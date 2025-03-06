#ifndef __DSHLIB_H__
    #define __DSHLIB_H__

#include <stdint.h>

//Constants for command structure sizes
#define EXE_MAX 64
#define ARG_MAX 256
#define CMD_MAX 8
#define CMD_ARGV_MAX (CMD_MAX + 1)
// Longest command that can be read from the shell
#define SH_CMD_MAX EXE_MAX + ARG_MAX

typedef struct cmd_buff
{
    int  argc;
    char *argv[CMD_ARGV_MAX];
    char *_cmd_buffer;
} cmd_buff_t;

/* WIP - Move to next assignment 
#define N_ARG_MAX    15     //MAX number of args for a command
typedef struct command{
    char exe [EXE_MAX];
    char args[ARG_MAX];
    int  argc;
    char *argv[N_ARG_MAX + 1];  //last argv[LAST] must be \0
}command_t;
*/

//Special character #defines
#define SPACE_CHAR  ' '
#define PIPE_CHAR   '|'
#define PIPE_STRING "|"

#define SH_PROMPT "dsh2> "
#define EXIT_CMD "exit"
#define CD_CMD "cd"
#define RC_CMD "rc"
// tmp Constants
#define EXIT_CMD_BYTES 4
#define DRAGON_CMD "dragon"
#define DRAGON_CMD_BYTES 6


//Standard Return Codes
#define OK                       0
#define WARN_NO_CMDS            -1
#define ERR_TOO_MANY_COMMANDS   -2
#define ERR_CMD_OR_ARGS_TOO_BIG -3
#define ERR_CMD_ARGS_BAD        -4      //for extra credit
#define ERR_MEMORY              -5
#define ERR_EXEC_CMD            -6
#define OK_EXIT                 -7

//prototypes
int alloc_cmd_buff(cmd_buff_t **cmd_buff);
int free_cmd_buff(cmd_buff_t **cmd_buff);
int clear_cmd_buff(cmd_buff_t *cmd_buff);
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff);
// dragon extra credit prototypes
extern void print_dragon();
int encode_dragon_ascii (char *ascii_art, uint8_t *x);
void decode_dragon_ascii (uint8_t *x, size_t length);

//built in command stuff
typedef enum {
    BI_CMD_EXIT,
    BI_CMD_DRAGON,
    BI_CMD_CD,
    BI_NOT_BI,
    BI_EXECUTED,
    BI_RC,
} Built_In_Cmds;
// Built_In_Cmds match_command(const char *input); 
// Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd);

//main execution context
int exec_local_cmd_loop();
// int exec_cmd(cmd_buff_t *cmd);




//output constants
#define CMD_OK_HEADER       "PARSED COMMAND LINE - TOTAL COMMANDS %d\n"
#define CMD_WARN_NO_CMD     "warning: no commands provided\n"
#define CMD_ERR_PIPE_LIMIT  "error: piping limited to %d commands\n"
















// drexel dragon
#define DRAGON_ASCII_ART \
    "                                                                        @%%%%                       \n"\
    "                                                                     %%%%%%                         \n"\
    "                                                                    %%%%%%                          \n"\
    "                                                                 % %%%%%%%           @              \n"\
    "                                                                %%%%%%%%%%        %%%%%%%           \n"\
    "                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        \n"\
    "                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          \n"\
    "                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           \n"\
    "                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            \n"\
    "                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            \n"\
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                \n"\
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"\
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              \n"\
    "      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                \n"\
    "    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                \n"\
    "  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        \n"\
    " %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       \n"\
    "%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     \n"\
    "%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  \n"\
    "%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"\
    "%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  \n"\
    "%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%\n"\
    "%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%\n"\
    "%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%\n"\
    " %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%\n"\
    "  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% \n"\
    "   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% \n"\
    "    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% \n"\
    "     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   \n"\
    "      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               \n"\
    "        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          \n"\
    "           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         \n"\
    "              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        \n"\
    "                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         \n"\
    "                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            \n"\
    "                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          \n"\
    "                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          \n"\
    "                                                                                 %%%%%%%@           \n\0"
#define DRAGON_ASCII_ART_LEN 960

#define SPACE 0b00
#define PERCENT 0b01 
#define AT 0b10
#define NEWLINE 0b11

#endif