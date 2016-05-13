#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "engine.h"

#define DEFAULT_INITIAL_VALUE   -1

#define UNIT_MOVED              0
#define ATTACKER_KILLED         1
#define DEFENDER_KILLED         2
#define BOTH_UNITS_DIED         3
#define ATTACKER_KILLED_KING    4
#define DEFENDER_KILLED_KING    5

#define NONE_POSESSION           0
#define BELONGS_TO_A            1
#define BELONGS_TO_B            2

#define PLAYER_A_TURN           1
#define PLAYER_B_TURN           2

#define PEASANT_PRODUCE_ID      0
#define KNIGHT_PRODUCE_ID       1

gameData currentGame;

void startGame() {
    currentGame.playerTurn = PLAYER_A_TURN;
    currentGame.gameMap = hashmapCreate();
    currentGame.currentRound = 1;
    currentGame.mapSize = DEFAULT_INITIAL_VALUE;
    currentGame.maxRound = DEFAULT_INITIAL_VALUE;
    currentGame.startingx1 = DEFAULT_INITIAL_VALUE;
    currentGame.startingx2 = DEFAULT_INITIAL_VALUE;
    currentGame.startingy1 = DEFAULT_INITIAL_VALUE;
    currentGame.startingy2 = DEFAULT_INITIAL_VALUE;
    currentGame.playerAinit = false;
    currentGame.playerBinit = false;

}

void endGame() {
    hashmapFree(currentGame.gameMap);
}

void printTopLeft() {
    for (int y = 1; y <= currentGame.mapSize; y++) {
        for (int x = 1; x <= currentGame.mapSize; x++) {
            printf("%c", getPawnSymbol(hashmapGet(currentGame.gameMap, x, y)));
        }
        printf("\n");
    }
    printf("\n");
}

int init(int n,
         int k,
         int p,
         int x1,
         int y1,
         int x2,
         int y2) {
    if (!validInitialization(n, k, p, x1, y1, x2, y2)) {
        return ERROR;
    }
    if (p == 1 && !currentGame.playerAinit) {
        currentGame.mapSize = n;
        currentGame.maxRound = k;
        currentGame.startingx1 = x1;
        currentGame.startingy1 = y1;
        currentGame.startingx2 = x2;
        currentGame.startingy2 = y2;
        currentGame.playerAinit = true;

        pawn *kingA = newPawn(x1, y1, currentGame.currentRound, KING_PLAYER_A_ID);
        pawn *peasantA = newPawn(x1 + 1, y1, currentGame.currentRound, PEASANT_PLAYER_A_ID);
        pawn *knight1A = newPawn(x1 + 2, y1, currentGame.currentRound, KNIGHT_PLAYER_A_ID);
        pawn *knight2A = newPawn(x1 + 3, y1, currentGame.currentRound, KNIGHT_PLAYER_A_ID);

        hashmapPut(currentGame.gameMap, kingA);
        hashmapPut(currentGame.gameMap, peasantA);
        hashmapPut(currentGame.gameMap, knight1A);
        hashmapPut(currentGame.gameMap, knight2A);

        pawn *kingB = newPawn(x2, y2, currentGame.currentRound, KING_PLAYER_B_ID);
        pawn *peasantB = newPawn(x2 + 1, y2, currentGame.currentRound, PEASANT_PLAYER_B_ID);
        pawn *knight1B = newPawn(x2 + 2, y2, currentGame.currentRound, KNIGHT_PLAYER_B_ID);
        pawn *knight2B = newPawn(x2 + 3, y2, currentGame.currentRound, KNIGHT_PLAYER_B_ID);

        hashmapPut(currentGame.gameMap, kingB);
        hashmapPut(currentGame.gameMap, peasantB);
        hashmapPut(currentGame.gameMap, knight1B);
        hashmapPut(currentGame.gameMap, knight2B);
    } else if (p == 2 && !currentGame.playerBinit) {
        currentGame.playerBinit = true;
    } else {
        return ERROR;
    }

    return GAME_OK;
}

int move(int x1,
         int y1,
         int x2,
         int y2) {
    if (!isValidField(currentGame.mapSize, x1, y1) ||
        !isValidField(currentGame.mapSize, x2, y2)) {
        return ERROR;
    }

    pawn *currentPawn = hashmapRemove(currentGame.gameMap, x1, y1);
    pawn *targetPawn = hashmapRemove(currentGame.gameMap, x2, y2);

    if (getPawnAdherence(currentPawn) == getPawnAdherence(targetPawn) ||
        (getPawnId(currentPawn) == KING_PLAYER_B_ID &&
         getPawnId(targetPawn) == KING_PLAYER_A_ID) ||
        (getPawnId(targetPawn) == KING_PLAYER_B_ID &&
         getPawnId(currentPawn) == KING_PLAYER_A_ID)) {
        return ERROR;
    } else {
        int actionResult = performAction(currentPawn, targetPawn);
        switch (actionResult) {
            case UNIT_MOVED:
                currentPawn->x = (unsigned int) x2 - 1;
                currentPawn->y = (unsigned int) y2 - 1;
                hashmapPut(currentGame.gameMap, currentPawn);
                break;
            case ATTACKER_KILLED:
                currentPawn->x = (unsigned int) x2 - 1;
                currentPawn->y = (unsigned int) y2 - 1;
                hashmapPut(currentGame.gameMap, currentPawn);
                free(targetPawn);
                break;
            case DEFENDER_KILLED:
                hashmapPut(currentGame.gameMap, targetPawn);
                free(currentPawn);
                break;
            case ATTACKER_KILLED_KING:
                free(currentPawn);
                free(targetPawn);
                return currentGame.playerTurn == PLAYER_A_TURN ? PLAYER_A_WON : PLAYER_B_WON;
            case DEFENDER_KILLED_KING:
                free(currentPawn);
                free(targetPawn);
                return currentGame.playerTurn == PLAYER_A_TURN ? PLAYER_B_WON : PLAYER_A_WON;
            case BOTH_UNITS_DIED:
                free(currentPawn);
                free(targetPawn);
                break;
            default:
                return ERROR;
        }
    }

    return GAME_OK;
}

int performAction(pawn *currentPawn,
                  pawn *targetPawn) {
    if (targetPawn == NULL) {
        return UNIT_MOVED;
    }

    if (currentPawn->id == KING_PLAYER_A_ID || currentPawn->id == KING_PLAYER_B_ID) {
        if (targetPawn->id == PEASANT_PLAYER_A_ID || targetPawn->id == PEASANT_PLAYER_B_ID) {
            return ATTACKER_KILLED;
        } else if (targetPawn->id == KNIGHT_PLAYER_A_ID || targetPawn->id == KNIGHT_PLAYER_B_ID) {
            return DEFENDER_KILLED_KING;
        } else {
            return ERROR;
        }
    } else if (currentPawn->id == PEASANT_PLAYER_A_ID || currentPawn->id == PEASANT_PLAYER_B_ID) {
        if (targetPawn->id == KING_PLAYER_A_ID || targetPawn->id == KING_PLAYER_B_ID) {
            return DEFENDER_KILLED;
        } else if (targetPawn->id == PEASANT_PLAYER_A_ID || targetPawn->id == PEASANT_PLAYER_B_ID) {
            return BOTH_UNITS_DIED;
        } else if (targetPawn->id == KNIGHT_PLAYER_A_ID || targetPawn->id == KNIGHT_PLAYER_B_ID) {
            return DEFENDER_KILLED;
        } else {
            return ERROR;
        }
    } else if (currentPawn->id == KNIGHT_PLAYER_A_ID || currentPawn->id == KNIGHT_PLAYER_B_ID) {
        if (targetPawn->id == KING_PLAYER_A_ID || targetPawn->id == KING_PLAYER_B_ID) {
            return ATTACKER_KILLED_KING;
        } else if (targetPawn->id == PEASANT_PLAYER_A_ID || targetPawn->id == PEASANT_PLAYER_B_ID) {
            return ATTACKER_KILLED;
        } else if (targetPawn->id == KNIGHT_PLAYER_A_ID || targetPawn->id == KNIGHT_PLAYER_B_ID) {
            return BOTH_UNITS_DIED;
        } else {
            return ERROR;
        }
    } else {
        return ERROR;
    }
}

int produceKnight(int x1,
                  int y1,
                  int x2,
                  int y2) {
    return produceUnit(x1, y1, x2, y2, KNIGHT_PRODUCE_ID);
}

int producePeasant(int x1,
                   int y1,
                   int x2,
                   int y2) {
    return produceUnit(x1, y1, x2, y2, PEASANT_PRODUCE_ID);
}

int produceUnit(int x1,
                int y1,
                int x2,
                int y2,
                int unitId) {
    if (!isValidField(currentGame.mapSize, x1, y1) ||
        !isValidField(currentGame.mapSize, x2, y2)) {
        return ERROR;
    }

    // TODO check if pawn has not moved for last 3 rounds.

    pawn *currentPawn = hashmapRemove(currentGame.gameMap, x1, x2);

    if (currentGame.playerTurn != getPawnAdherence(currentPawn)) {
        return ERROR;
    }

    if (getPawnId(currentPawn) == PEASANT_PLAYER_A_ID ||
        getPawnId(currentPawn) == PEASANT_PLAYER_B_ID) {
        if (getPawnId(currentPawn) == EMPTY_SPACE_ID) {
            pawn *createdPawn;
            if (unitId == PEASANT_PRODUCE_ID) {
                createdPawn = newPawn(x2, y2, currentGame.currentRound,
                                      (currentGame.currentRound == PLAYER_A_TURN) ?
                                      PEASANT_PLAYER_A_ID : PEASANT_PLAYER_B_ID);
            } else if (unitId == KNIGHT_PRODUCE_ID) {
                createdPawn = newPawn(x2, y2, currentGame.currentRound,
                                      (currentGame.currentRound == PLAYER_A_TURN) ?
                                      KNIGHT_PLAYER_A_ID : KNIGHT_PLAYER_B_ID);
            } else {
                return ERROR;
            }
            hashmapPut(currentGame.gameMap, createdPawn);
            return GAME_OK;
        } else {
            return ERROR;
        }
    } else {
        return ERROR;
    }
}

int endTurn() {
    if (currentGame.playerTurn == PLAYER_A_TURN) {
        currentGame.playerTurn = PLAYER_B_TURN;
    } else {

        currentGame.playerTurn = PLAYER_A_TURN;
        currentGame.currentRound++;
    }

    if (currentGame.currentRound > currentGame.maxRound) {
        return DRAW;
    }

    return GAME_OK;
}

int max(int a,
        int b) {
    return a > b ? a : b;
}

int distMax(int x1,
            int y1,
            int x2,
            int y2) {
    return max(abs(x1 - x2), abs(y1 - y2));
}

bool isValidField(int mapSize,
                  int x,
                  int y) {
    bool isValid = true;

    if (x < 1 || x > mapSize || y < 1 || y > mapSize) {
        isValid = false;
    }

    return isValid;
}

bool validInitialization(int n,
                         int k,
                         int p,
                         int x1,
                         int y1,
                         int x2,
                         int y2) {
    bool isValid = true;

    if (currentGame.mapSize != DEFAULT_INITIAL_VALUE && n != currentGame.mapSize) {
        isValid = false;
    }

    if (currentGame.maxRound != DEFAULT_INITIAL_VALUE && k != currentGame.maxRound) {
        isValid = false;
    }
    if (currentGame.startingx1 != DEFAULT_INITIAL_VALUE && x1 != currentGame.startingx1) {
        isValid = false;
    }
    if (currentGame.startingy1 != DEFAULT_INITIAL_VALUE && y1 != currentGame.startingy1) {
        isValid = false;
    }
    if (currentGame.startingx2 != DEFAULT_INITIAL_VALUE && x2 != currentGame.startingx2) {
        isValid = false;
    }
    if (currentGame.startingy2 != DEFAULT_INITIAL_VALUE && y2 != currentGame.startingy2) {
        isValid = false;
    }
    if (x1 < 1 || x1 > n || y1 < 1 || y1 > n || x2 < 1 || x2 > n || y2 < 1 || y2 > n) {
        isValid = false;
    }
    if (distMax(x1, y1, x2, y2) < 8) {
        isValid = false;
    }
    if (n <= 8 || n >= pow(2, 31)) {
        isValid = false;
    }
    if (p != 1 && p != 2) {
        isValid = false;
    }

    return isValid;
}

int getPawnAdherence(pawn *currentPawn) {
    if (currentPawn == NULL) {
        return NONE_POSESSION;
    }
    switch (currentPawn->id) {
        case KING_PLAYER_A_ID:
        case KNIGHT_PLAYER_A_ID:
        case PEASANT_PLAYER_A_ID:
            return BELONGS_TO_A;
        case KING_PLAYER_B_ID:
        case KNIGHT_PLAYER_B_ID:
        case PEASANT_PLAYER_B_ID:
            return BELONGS_TO_B;
        case EMPTY_SPACE_ID:
        default:
            return NONE_POSESSION;
    }
}
