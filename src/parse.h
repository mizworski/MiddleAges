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
    int command_id;
    int data[7];
} command;


/** Reads a command.
  Returns command with data points using 'command' structure.
  */
command* parse_command();

#endif /* PARSE_H */
