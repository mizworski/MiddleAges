/** @file
   Interface of parser.

*/

#ifndef PARSE_H
#define PARSE_H

#define PARSE_ERROR         -1
#define INIT                0
#define MOVE                1
#define PRODUCE_KNIGHT      2
#define PRODUCE_PEASANT     3
#define END_TURN            4

#define MAX_ARGS_COUNT      7

typedef struct def_command {
    int commandId;
    int data[MAX_ARGS_COUNT];
} command;

static int getArgumentsFromString(char *bufferedString,
                           command *newCommand);

static int getCommandCode(char *commandString,
                   int *charsShiftInString);

static int getReadLineLength(char *lineRead);

/** Reads a command.
  Returns command with hashArrayOfLists points using 'command' structure.
  */
command *parseCommand();

#endif /* PARSE_H */