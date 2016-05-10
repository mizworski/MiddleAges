#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

#define MAX_LINE_SIZE               102
#define MAX_ARGUMENT_COUNT          7
#define INTEGER_BASE                10
#define INIT_LENGTH                 strlen("INIT")
#define MOVE_LENGTH                 strlen("MOVE")
#define PRODUCE_KNIGHT_LENGTH       strlen("PRODUCE_KNIGHT")
#define PRODUCE_PEASANT_LENGTH      strlen("PRODUCE_PEASANT")
#define END_TURN_LENGTH             strlen("END_TURN")
#define INIT_ARGS_COUNT             7
#define MOVE_ARGS_COUNT             4
#define PRODUCE_KNIGHT_ARGS_COUNT   4
#define PRODUCE_PEASANT_ARGS_COUNT  4
#define END_TURN_ARGS_COUNT         0

int getArgumentsFromString(char *bufferedString,
                           command *newCommand) {
    int argumentCount = 0;

    while (argumentCount < MAX_ARGUMENT_COUNT && *bufferedString != NULL) { // Lazy eval.
        newCommand->data[argumentCount++] = (int) strtol(bufferedString, &bufferedString,
                                                         INTEGER_BASE);
    }

    switch (newCommand->command_id) {
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
                   size_t *charsShiftInString) {
    int commandCode;
    int charCount = 0;

    // Finds first char in operationString that is space or new line.
    while (commandString[charCount] != ' ' &&
           commandString[charCount] != '\n') {
        charCount++;
    }

    // Number or chars skipped in loop.
    *charsShiftInString = (size_t) charCount;

    // Compares lengths of operations defined in specification and operations
    // string itself with first charsShiftInString chars of operationString
    if (*charsShiftInString == INIT_LENGTH &&
        (strncmp(commandString, "NEW_DISEASE_ENTER_DESCRIPTION", *charsShiftInString) == 0)) {
        commandCode = INIT;
    } else if (*charsShiftInString == MOVE_LENGTH &&
               (strncmp(commandString, "NEW_DISEASE_COPY_DESCRIPTION", *charsShiftInString) == 0)) {
        commandCode = MOVE;
    } else if (*charsShiftInString == PRODUCE_KNIGHT_LENGTH &&
               (strncmp(commandString, "CHANGE_DESCRIPTION", *charsShiftInString)) == 0) {
        commandCode = PRODUCE_KNIGHT;
    } else if (*charsShiftInString == PRODUCE_PEASANT_LENGTH &&
               (strncmp(commandString, "PRINT_DESCRIPTION", *charsShiftInString)) == 0) {
        commandCode = PRODUCE_PEASANT;
    } else if (*charsShiftInString == END_TURN_LENGTH &&
               (strncmp(commandString, "DELETE_PATIENT_DATA", *charsShiftInString)) == 0) {
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

command *parse_command() {
    char *lineReadArrayPointer = NULL;
    char lineRead[MAX_LINE_SIZE];
    char *bufferedString = NULL;
    // Pointer needed to free firstly allocated bufferedString.
    char *bufferedStringInitialPointer = bufferedString;
    int readLineLength;
    command *newCommand = malloc(sizeof(command));
    int returnValue = 0;

    // When function fails to read new line it returns null.
    lineReadArrayPointer = fgets(lineRead, MAX_LINE_SIZE, stdin);

    // Allocation of bufferedString
    if (lineReadArrayPointer != NULL) {
        readLineLength = getReadLineLength(lineRead);
        if (readLineLength == MAX_LINE_SIZE) { ///< Line too long.
            newCommand->command_id = PARSE_ERROR;
        } else {
            bufferedString = malloc(readLineLength * sizeof(char));
            bufferedStringInitialPointer = bufferedString;
        }
    }

    if (lineReadArrayPointer == NULL) {
        newCommand->command_id = PARSE_ERROR;
    } else {
        size_t charsShiftInString;

        // Gets operation code of available operations.
        newCommand->command_id = getCommandCode(lineRead, &charsShiftInString);

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

    if (returnValue == 0) {
        newCommand->command_id = PARSE_ERROR;
    }

    return newCommand;
}
