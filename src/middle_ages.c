#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "engine.h"

#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE     42

#define PLAYER_A_WON_MESSAGE                        "player 1 won\n"
#define PLAYER_B_WON_MESSAGE                        "player 2 won\n"
#define INPUT_ERROR_MESSAGE                         "input error\n"
#define DRAW_MESSAGE                                "draw\n"

static void changePlayer(int *a) {
    if (*a == 1) {
        *a = 2;
    } else {
        *a = 1;
    }
}

int main() {
    bool gameOver = false; ///<  Variable that indicate whether to terminate loop.
    int currentPlayer = 1;
    int myNumber = 0;

    startGame();

    command *newCommand;

    while (!gameOver) {
        int returnValue = 0;

        newCommand = parseCommand();

        switch (newCommand->commandId) {
            case PARSE_ERROR:
                returnValue = ERROR;
                break;
            case INIT:
                returnValue = init(newCommand->data[0], newCommand->data[1],
                                   newCommand->data[2], newCommand->data[3],
                                   newCommand->data[4], newCommand->data[5],
                                   newCommand->data[6]);
                myNumber = newCommand->data[2];
                break;
            case MOVE:
                returnValue = move(newCommand->data[0], newCommand->data[1],
                                   newCommand->data[2], newCommand->data[3]);
                break;
            case PRODUCE_KNIGHT:
                returnValue = produceKnight(newCommand->data[0], newCommand->data[1],
                                            newCommand->data[2], newCommand->data[3]);
                break;
            case PRODUCE_PEASANT:
                returnValue = producePeasant(newCommand->data[0], newCommand->data[1],
                                             newCommand->data[2], newCommand->data[3]);
                break;
            case END_TURN:
                returnValue = endTurn();
                changePlayer(&currentPlayer);
                break;
            default:
                returnValue = ERROR;
        }

        if (currentPlayer == myNumber) {
            makeMoves();
        }

        free(newCommand); ///< Freeing command after passing it to engine.

        if (returnValue == ERROR) {
            endGame();
            fprintf(stderr,
                    INPUT_ERROR_MESSAGE);
            return ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE;
        }
/*
        if (returnValue != END_TURN_RETURN_VALUE && returnValue != EXCEEDED_ROUND_LIMIT) {
            printTopLeft();
        }
*/

        if (returnValue == DRAW || returnValue == EXCEEDED_ROUND_LIMIT) {
            gameOver = true;
            fprintf(stderr,
                    DRAW_MESSAGE);
        }

        if (returnValue == PLAYER_A_WON) {
            gameOver = true;
            fprintf(stderr,
                    PLAYER_A_WON_MESSAGE);
        }
        if (returnValue == PLAYER_B_WON) {
            gameOver = true;
            fprintf(stderr,
                    PLAYER_B_WON_MESSAGE);
        }
    }

    endGame();

    return 0;
}