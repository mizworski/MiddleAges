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

int getArgumentsFromString(char *bufferedString,
                           command *newCommand) {
    int argumentCount = 0;
    char lastDigit = '\0';
    while (argumentCount < MAX_ARGUMENT_COUNT && bufferedString != NULL &&
           *bufferedString != '\n' && *bufferedString != '\0' && isdigit(*bufferedString)) {
        newCommand->data[argumentCount++] = (int) strtol(bufferedString, &bufferedString,
                                                         INTEGER_BASE);
        lastDigit = *bufferedString;
        bufferedString++;
    }

    if (lastDigit != '\n' && lastDigit != '\0') {
        return PARSE_ERROR;
    }

    switch (newCommand->commandId) {
        case PARSE_ERROR:
            return -1;
        case INIT:
            if (argumentCount != INIT_ARGS_COUNT) {
                return -1;
            }
            break;
        case MOVE:
            if (argumentCount != MOVE_ARGS_COUNT) {
                return -1;
            }
            break;
        case PRODUCE_KNIGHT:
            if (argumentCount != PRODUCE_KNIGHT_ARGS_COUNT) {
                return -1;
            }
            break;
        case PRODUCE_PEASANT:
            if (argumentCount != PRODUCE_PEASANT_ARGS_COUNT) {
                return -1;
            }
            break;
        case END_TURN:
            if (argumentCount != END_TURN_ARGS_COUNT) {
                return -1;
            }
            break;
        default:
            return -1;
    }

    return 0;
}

int getCommandCode(char *commandString,
                   int *charsShiftInString) {
    int commandCode;
    int charCount = 0;

    // Finds first char in operationString that is space or new line.
    while (commandString[charCount] != ' ' &&
           commandString[charCount] != '\n') {
        charCount++;
    }

    // Number or chars skipped in loop.
    *charsShiftInString = charCount; //BLAD

    // Compares lengths of operations defined in specification and operations
    // string itself with first charsShiftInString chars of operationString
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

    // Identifies which procedure was called.
    return commandCode;
}

int getReadLineLength(char *lineRead) {
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
    // Pointer needed to free firstly allocated bufferedString.
    char *bufferedStringInitialPointer = bufferedString;
    int readLineLength = 0;
    command *newCommand = malloc(sizeof(command));
    newCommand->commandId = 0;
    int returnValue = 0;
    // TODO still waiting
    // When function fails to read new line it returns null.
    lineReadArrayPointer = fgets(lineRead, MAX_LINE_SIZE, stdin);
    // Allocation of bufferedString
    if (lineReadArrayPointer != NULL) {
        readLineLength = getReadLineLength(lineRead);
        if (readLineLength == MAX_LINE_SIZE) { ///< Line too long.
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
        strcpy(bufferedString, lineReadArrayPointer + charsShiftInString);

        returnValue = getArgumentsFromString(bufferedString,
                                             newCommand);
    }

    // Frees buffer that is not needed anymore.
    free(bufferedStringInitialPointer);

    if (returnValue == PARSE_ERROR) {
        newCommand->commandId = PARSE_ERROR;
    }

    return newCommand;
}
