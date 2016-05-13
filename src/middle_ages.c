#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "engine.h"

#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE     42

#define PLAYER_A_WON_MESSAGE    "player 1 won"
#define PLAYER_B_WON_MESSAGE    "player 2 won"
#define INPUT_ERROR_MESSAGE     "input error"
#define DRAW_MESSAGE            "draw"

int main() {
    bool gameOver = false;
    startGame();

    command *newCommand;

    while (!gameOver) {
        newCommand = parseCommand();
        int returnValue = 0;
        switch (newCommand->commandId) {
            case PARSE_ERROR:
                returnValue = ERROR;
                break;
            case INIT:
                returnValue = init(newCommand->data[0], newCommand->data[1],
                                   newCommand->data[2], newCommand->data[3],
                                   newCommand->data[4], newCommand->data[5],
                                   newCommand->data[6]);
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
                break;
            default:
                returnValue = ERROR;
        }

        free(newCommand);

        if (returnValue == ERROR) {
            endGame();
            fprintf(stderr,
                    INPUT_ERROR_MESSAGE);
            return ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE;
        }

        if (returnValue == DRAW) {
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

        printTopLeft();
    }

    endGame();

    return 0;
}
