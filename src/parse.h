 /** @file
    Interface of parser.

 */

#ifndef PARSE_H
#define PARSE_H

typedef struct def_command {
    char name[16];
    int data[7];
} command;


/** Reads a command.
  Returns command with data points using 'command' structure.
  */
command* parse_command();

#endif /* PARSE_H */
