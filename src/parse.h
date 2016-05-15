/** @file
   Interface of parser.

*/

#ifndef PARSE_H
#define PARSE_H

#define PARSE_ERROR             -1
#define PARSE_OK                0

#define INIT                    0
#define MOVE                    1
#define PRODUCE_KNIGHT          2
#define PRODUCE_PEASANT         3
#define END_TURN                4

#define MAX_ARGS_COUNT          7

typedef struct def_command {
    int commandId;              ///< Command identifier.
    int data[MAX_ARGS_COUNT];   ///< Array of arguments.
} command;

/**
 * @brief                       Gets arguments from a string.
 *
 * @param bufferedString        String with arguments to read.
 * @param *newCommand           Structure that loads arguments.
 * @return                      PARSE_ERROR upon wrong argument count
 *                              or PARSE_OK if read was successful.
 */
static int getArgumentsFromString(char *bufferedString,
                                  command *newCommand);

/**
 * @brief Gets command code from string, which identifies which command was read.
 *
 * @param *commandString        String with command to read.
 * @param *charsShiftInString   Command length.
 * @return                      Command code.
 */
static int getCommandCode(char *commandString,
                          int *charsShiftInString);

/**
 * @brief                       Gets string length.
 *
 * @param *lineRead             String that length is calculated.
 * @return                      Length of string.
 */
static int getReadLineLength(char *lineRead);

/**
 * @brief                       Reads command.
 *
 * @return                      structure with command identifier and arguments of that command.
 */
command *parseCommand();

#endif /* PARSE_H */