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

typedef struct def_command {
    int commandId;
    int data[7];
} command;

int getArgumentsFromString(char *bufferedString,
                           command *newCommand);

int getCommandCode(char *commandString,
                   int *charsShiftInString);

int getReadLineLength(char *lineRead);

/** Reads a command.
  Returns command with hashArrayOfLists points using 'command' structure.
  */
command *parseCommand();

#endif /* PARSE_H */