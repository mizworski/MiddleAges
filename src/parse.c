#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

#define MAX_LINE_SIZE               102
#define MAX_ARGUMENT_COUNT          7
#define INTEGER_BASE                10

#define INIT_ARGS_COUNT             7
#define MOVE_ARGS_COUNT             4
#define PRODUCE_KNIGHT_ARGS_COUNT   4
#define PRODUCE_PEASANT_ARGS_COUNT  4
#define END_TURN_ARGS_COUNT         0

#define INIT_LENGTH                 strlen("INIT")
#define MOVE_LENGTH                 strlen("MOVE")
#define PRODUCE_KNIGHT_LENGTH       strlen("PRODUCE_KNIGHT")
#define PRODUCE_PEASANT_LENGTH      strlen("PRODUCE_PEASANT")
#define END_TURN_LENGTH             strlen("END_TURN")

static int getArgumentsFromString(char *bufferedString,
                                  command *newCommand) {
    int argumentCount = 0;
    char lastDigit = '\0';
    while (argumentCount < MAX_ARGUMENT_COUNT &&
           bufferedString != NULL &&
           *bufferedString != '\n' &&
           *bufferedString != '\0' &&
           isdigit(*bufferedString)) {              ///< Checks if first char of string is digit.
        newCommand->data[argumentCount++] = (int) strtol(bufferedString, &bufferedString, INTEGER_BASE);

        lastDigit = *bufferedString;
        bufferedString++;               ///< Skips whitespace.
    }

    if (lastDigit != '\n' && lastDigit != '\0') {
        return PARSE_ERROR;
    }

    ///< Checks if correct number of arguments were read.
    switch (newCommand->commandId) {
        case PARSE_ERROR:
            return PARSE_ERROR;
        case INIT:
            if (argumentCount != INIT_ARGS_COUNT) {
                return PARSE_ERROR;
            }
            break;
        case MOVE:
            if (argumentCount != MOVE_ARGS_COUNT) {
                return PARSE_ERROR;
            }
            break;
        case PRODUCE_KNIGHT:
            if (argumentCount != PRODUCE_KNIGHT_ARGS_COUNT) {
                return PARSE_ERROR;
            }
            break;
        case PRODUCE_PEASANT:
            if (argumentCount != PRODUCE_PEASANT_ARGS_COUNT) {
                return PARSE_ERROR;
            }
            break;
        case END_TURN:
            if (argumentCount != END_TURN_ARGS_COUNT) {
                return PARSE_ERROR;
            }
            break;
        default:
            return PARSE_ERROR;
    }

    return PARSE_OK;
}

static int getCommandCode(char *commandString,
                          int *charsShiftInString) {
    int commandCode;
    int charCount = 0;

    /// Finds first char in operationString that is space or new line.
    while (commandString[charCount] != ' ' &&
           commandString[charCount] != '\n') {
        charCount++;
    }

    *charsShiftInString = charCount; ///< Number or chars skipped in loop.

    /// Compares lengths of operations defined in specification and operations
    /// string itself with first charsShiftInString chars of operationString
    if (*charsShiftInString == INIT_LENGTH &&
        (strncmp(commandString, "INIT", (size_t) *charsShiftInString) == 0)) {
        commandCode = INIT;
    } else if (*charsShiftInString == MOVE_LENGTH &&
               (strncmp(commandString, "MOVE", (size_t) *charsShiftInString) == 0)) {
        commandCode = MOVE;
    } else if (*charsShiftInString == PRODUCE_KNIGHT_LENGTH &&
               (strncmp(commandString, "PRODUCE_KNIGHT", (size_t) *charsShiftInString)) == 0) {
        commandCode = PRODUCE_KNIGHT;
    } else if (*charsShiftInString == PRODUCE_PEASANT_LENGTH &&
               (strncmp(commandString, "PRODUCE_PEASANT", (size_t) *charsShiftInString)) == 0) {
        commandCode = PRODUCE_PEASANT;
    } else if (*charsShiftInString == END_TURN_LENGTH &&
               (strncmp(commandString, "END_TURN", (size_t) *charsShiftInString)) == 0) {
        commandCode = END_TURN;
    } else {
        commandCode = PARSE_ERROR;
    }

    // Adds one to get position of first char after operation.
    (*charsShiftInString)++;

    // Identifies which command was called.
    return commandCode;
}

static int getReadLineLength(char *lineRead) {
    int charCount = 0;

    while (lineRead[charCount] != '\n' && charCount < MAX_LINE_SIZE) {
        charCount++;
    }

    return charCount;
}

command *parseCommand() {
    char *lineReadArrayPointer = NULL;
    char lineRead[MAX_LINE_SIZE];
    char *bufferedString = NULL;
    char *bufferedStringInitialPointer = bufferedString;
    int readLineLength = 0;
    command *newCommand = malloc(sizeof(command));

    newCommand->commandId = 0;
    for (int i = 0; i < MAX_ARGUMENT_COUNT; i++) {
        newCommand->data[i] = 0;
    }
    int returnValue = 0;

    // When function fails to read new line it returns null.
    lineReadArrayPointer = fgets(lineRead, MAX_LINE_SIZE, stdin);
    // Allocation of bufferedString
    if (lineReadArrayPointer != NULL) {
        readLineLength = getReadLineLength(lineRead);
        if (readLineLength == MAX_LINE_SIZE || readLineLength == 0) { ///< Line too long or empty.
            newCommand->commandId = PARSE_ERROR;
        } else {
            bufferedString = malloc(readLineLength * sizeof(char));
            bufferedStringInitialPointer = bufferedString;
        }
    }

    if (lineReadArrayPointer == NULL) {
        newCommand->commandId = PARSE_ERROR;
    } else if (newCommand->commandId != PARSE_ERROR) {
        int charsShiftInString;

        // Gets operation code of available operations.
        newCommand->commandId = getCommandCode(lineRead, &charsShiftInString);

        // Copies lineRead shifted by number of characters, that was used by
        // procedure call, to bufferedString.
        // Safe to use because bufferString size is not greater than
        // MAX_LINE_SIZE
        if (newCommand->commandId != PARSE_ERROR) {
            strcpy(bufferedString, lineReadArrayPointer + charsShiftInString);
            returnValue = getArgumentsFromString(bufferedString,
                                                 newCommand);
        }
    }

    // Frees buffer that is not needed anymore.
    if (bufferedStringInitialPointer != NULL) {
        free(bufferedStringInitialPointer);
    }

    if (returnValue == PARSE_ERROR) {
        newCommand->commandId = PARSE_ERROR;
    }

    return newCommand;
}
