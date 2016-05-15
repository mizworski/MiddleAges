#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "engine.h"

#define DEFAULT_INITIAL_VALUE   -1

#define MIN_INITIAL_DIST        8
#define MIN_MAP_SIZE            8
#define MAX_MAP_SIZE            pow(2, 31)
#define MAX_ROUND_LIMIT         pow(2, 31)

#define UNIT_MOVED              0
#define ATTACKER_KILLED         1
#define DEFENDER_KILLED         2
#define BOTH_UNITS_DIED         3
#define ATTACKER_KILLED_KING    4
#define DEFENDER_KILLED_KING    5

#define EMPTY_FIELD             0
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
    currentGame.startingX1 = DEFAULT_INITIAL_VALUE;
    currentGame.startingX2 = DEFAULT_INITIAL_VALUE;
    currentGame.startingY1 = DEFAULT_INITIAL_VALUE;
    currentGame.startingY2 = DEFAULT_INITIAL_VALUE;
    currentGame.hasPlayerAInitialized = false;
    currentGame.hasPlayerBInitialized = false;
}

void endGame() {
    hashmapFree(currentGame.gameMap);
}

void printTopLeft() {
    for (int y = 1; y <= currentGame.mapSize && y <= 10; y++) {
        for (int x = 1; x <= currentGame.mapSize && x <= 10; x++) {
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
    /// Validates initialization.
    if (!validInitialization(n, k, p, x1, y1, x2, y2)) {
        return ERROR;
    }

    /// Checks if first player initialized game already.
    if (p == 1 && currentGame.hasPlayerAInitialized) {
        return ERROR;
    }

    /// Checks if second player initialized game already.
    if (p == 2 && currentGame.hasPlayerBInitialized) {
        return ERROR;
    }

    if (!currentGame.hasPlayerAInitialized && !currentGame.hasPlayerBInitialized) {
        currentGame.mapSize = n;
        currentGame.maxRound = k;
        currentGame.startingX1 = x1;
        currentGame.startingY1 = y1;
        currentGame.startingX2 = x2;
        currentGame.startingY2 = y2;

        /// Initializes last round to 0, so pawns can move immediately.
        /// Places them on 4 fields next to each other in same row, starting from king.
        pawn *kingA = newPawn(x1, y1, currentGame.currentRound - 1, KING_PLAYER_A_ID);
        pawn *peasantA = newPawn(x1 + 1, y1, currentGame.currentRound - 1, PEASANT_PLAYER_A_ID);
        pawn *knight1A = newPawn(x1 + 2, y1, currentGame.currentRound - 1, KNIGHT_PLAYER_A_ID);
        pawn *knight2A = newPawn(x1 + 3, y1, currentGame.currentRound - 1, KNIGHT_PLAYER_A_ID);

        hashmapPut(currentGame.gameMap, kingA);
        hashmapPut(currentGame.gameMap, peasantA);
        hashmapPut(currentGame.gameMap, knight1A);
        hashmapPut(currentGame.gameMap, knight2A);

        pawn *kingB = newPawn(x2, y2, currentGame.currentRound - 1, KING_PLAYER_B_ID);
        pawn *peasantB = newPawn(x2 + 1, y2, currentGame.currentRound - 1, PEASANT_PLAYER_B_ID);
        pawn *knight1B = newPawn(x2 + 2, y2, currentGame.currentRound - 1, KNIGHT_PLAYER_B_ID);
        pawn *knight2B = newPawn(x2 + 3, y2, currentGame.currentRound - 1, KNIGHT_PLAYER_B_ID);

        hashmapPut(currentGame.gameMap, kingB);
        hashmapPut(currentGame.gameMap, peasantB);
        hashmapPut(currentGame.gameMap, knight1B);
        hashmapPut(currentGame.gameMap, knight2B);
    }

    if (p == 1 && !currentGame.hasPlayerAInitialized) {
        currentGame.hasPlayerAInitialized = true;
    }

    if (p == 2 && !currentGame.hasPlayerBInitialized) {
        currentGame.hasPlayerBInitialized = true;
    }

    if (p != 1 && p != 2) { ///< Wrong player initialization.
        return ERROR;
    }

    return GAME_OK;
}

int move(int x1,
         int y1,
         int x2,
         int y2) {
    /// Validates fields.
    if (!isValidField(currentGame.mapSize, x1, y1) ||
        !isValidField(currentGame.mapSize, x2, y2)) {
        return ERROR;
    }

    /// Validates initialization.
    if (currentGame.hasPlayerAInitialized == false ||
        currentGame.hasPlayerBInitialized == false) {
        return ERROR;
    }

    /// Validates distance between fields.
    if (distMax(x1, y1, x2, y2) != 1) {
        return ERROR;
    }

    /// Picks pawn from board.
    pawn *currentPawn = hashmapRemove(currentGame.gameMap, x1, y1);

    /// Checks if any pawn was picked.
    if (currentPawn == NULL) {
        return ERROR;
    }

    /// Checks if pawn is able to move.
    if (currentPawn->lastMove >= currentGame.currentRound) {
        free(currentPawn);
        return ERROR;
    }

    /// Checks if pawn belong to current player.
    if (currentGame.playerTurn != getPawnAdherence(currentPawn)) {
        free(currentPawn);
        return ERROR;
    }

    currentPawn->lastMove = currentGame.currentRound;

    pawn *targetPawn = hashmapRemove(currentGame.gameMap, x2, y2);

    if (getPawnAdherence(currentPawn) == getPawnAdherence(targetPawn)) {
        ///< Player wants to move onto his own pawn.
        free(currentPawn);
        if (targetPawn != NULL) {
            free(targetPawn);
        }
        return ERROR;
    } else {
        int actionResult = performAction(currentPawn, targetPawn);
        switch (actionResult) {
            /// Frees pawn that was killed.
            /// Puts pawn that survived.
            case UNIT_MOVED:
                currentPawn->x = (unsigned int) x2 - 1; ///< 1-based to 0-based.
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
                currentPawn->x = (unsigned int) x2 - 1;
                currentPawn->y = (unsigned int) y2 - 1;
                hashmapPut(currentGame.gameMap, currentPawn);
                free(targetPawn);
                /// Player that made move is victorious.
                return currentGame.playerTurn == PLAYER_A_TURN ? PLAYER_A_WON : PLAYER_B_WON;
            case DEFENDER_KILLED_KING:
                free(currentPawn);
                hashmapPut(currentGame.gameMap, targetPawn);
                /// Player that made move is beaten.
                return currentGame.playerTurn == PLAYER_A_TURN ? PLAYER_B_WON : PLAYER_A_WON;
            case BOTH_UNITS_DIED:
                if (isKing(currentPawn)) {
                    free(currentPawn);
                    free(targetPawn);
                    return DRAW; ///< Kings killed each other.
                } else {
                    free(currentPawn);
                    free(targetPawn);
                }
                break;
            default:
                return ERROR;
        }
    }

    return GAME_OK;
}

static int performAction(pawn *currentPawn,
                         pawn *targetPawn) {
    if (targetPawn == NULL) { ///< Targeted field was empty.
        return UNIT_MOVED;
    }

    if (isKing(currentPawn)) {
        if (isKing(targetPawn)) {
            return BOTH_UNITS_DIED;
        } else if (isPeasant(targetPawn)) {
            return ATTACKER_KILLED;
        } else if (isKnight(targetPawn)) {
            return DEFENDER_KILLED_KING;
        } else {
            return ERROR;
        }
    } else if (isPeasant(currentPawn)) {
        if (isKing(targetPawn)) {
            return DEFENDER_KILLED;
        } else if (isPeasant(targetPawn)) {
            return BOTH_UNITS_DIED;
        } else if (isKnight(targetPawn)) {
            return DEFENDER_KILLED;
        } else {
            return ERROR;
        }
    } else if (isKnight(currentPawn)) {
        if (isKing(targetPawn)) {
            return ATTACKER_KILLED_KING;
        } else if (isPeasant(targetPawn)) {
            return ATTACKER_KILLED;
        } else if (isKnight(targetPawn)) {
            return BOTH_UNITS_DIED;
        } else {
            return ERROR;
        }
    } else {
        return ERROR;
    }
}

static bool isKing(pawn *currentPawn) {
    return currentPawn->id == KING_PLAYER_A_ID || currentPawn->id == KING_PLAYER_B_ID;
}

static bool isKnight(pawn *currentPawn) {
    return currentPawn->id == KNIGHT_PLAYER_A_ID || currentPawn->id == KNIGHT_PLAYER_B_ID;
}

static bool isPeasant(pawn *currentPawn) {
    return currentPawn->id == PEASANT_PLAYER_A_ID || currentPawn->id == PEASANT_PLAYER_B_ID;
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

static int produceUnit(int x1,
                       int y1,
                       int x2,
                       int y2,
                       int unitId) {
    /// Validates initialization.
    if (!isValidField(currentGame.mapSize, x1, y1) ||
        !isValidField(currentGame.mapSize, x2, y2)) {
        return ERROR;
    }

    if (currentGame.hasPlayerAInitialized == false ||
        currentGame.hasPlayerBInitialized == false) {
        return ERROR;
    }

    /// Validates distance.
    if (distMax(x1, y1, x2, y2) != 1) {
        return ERROR;
    }

    /// Picks pawn from board.
    pawn *currentPawn = hashmapGet(currentGame.gameMap, x1, y1);

    /// Checks if pawn was picked.
    if (currentPawn == NULL) {
        return ERROR;
    }

    /// Checks if pawn is able to produce.
    if (currentPawn->lastMove >= currentGame.currentRound - 2) {
        return ERROR;
    }

    /// Checks if pawn belongs to current player.
    if (currentGame.playerTurn != getPawnAdherence(currentPawn)) {
        return ERROR;
    }

    if (isPeasant(currentPawn)) {
        pawn *targetPawn = hashmapGet(currentGame.gameMap, x2, y2);
        if (getPawnId(targetPawn) == EMPTY_SPACE_ID) {
            pawn *createdPawn;
            currentPawn->lastMove = currentGame.currentRound;
            if (unitId == PEASANT_PRODUCE_ID) {
                int newPawnId = currentGame.playerTurn == PLAYER_A_TURN ?
                                PEASANT_PLAYER_A_ID : PEASANT_PLAYER_B_ID;
                createdPawn = newPawn(x2, y2, currentGame.currentRound - 1, newPawnId);
            } else if (unitId == KNIGHT_PRODUCE_ID) {
                int newPawnId = currentGame.playerTurn == PLAYER_A_TURN ?
                                KNIGHT_PLAYER_A_ID : KNIGHT_PLAYER_B_ID;
                createdPawn = newPawn(x2, y2, currentGame.currentRound - 1, newPawnId);
            } else { ///< Tried to produce wrong unit.
                return ERROR;
            }
            hashmapPut(currentGame.gameMap, createdPawn);
            return GAME_OK;
        } else { ///< Field was not empty.
            return ERROR;
        }
    } else { ///< Unit that tried to produce was not peasant.
        return ERROR;
    }
}

int endTurn() {
    /// Validates initialization.
    if (currentGame.hasPlayerAInitialized == false ||
        currentGame.hasPlayerBInitialized == false) {
        return ERROR;
    }

    /// Changes player turn.
    if (currentGame.playerTurn == PLAYER_A_TURN) {
        currentGame.playerTurn = PLAYER_B_TURN;
    } else {
        currentGame.playerTurn = PLAYER_A_TURN;
        currentGame.currentRound++; ///< Both players ended turns. New round starts.
    }

    if (currentGame.currentRound > currentGame.maxRound) {
        return EXCEEDED_ROUND_LIMIT;
    }

    return END_TURN_RETURN_VALUE;
}

static int max(int a,
               int b) {
    return a > b ? a : b;
}

static int distMax(int x1,
                   int y1,
                   int x2,
                   int y2) {
    return max(abs(x1 - x2), abs(y1 - y2));
}


static bool isValidField(int mapSize,
                         int x,
                         int y) {
    bool isValid = true;

    /// Checks if field is out of map bounds.
    if (x < 1 || x > mapSize || y < 1 || y > mapSize) {
        isValid = false;
    }

    return isValid;
}

static bool validInitialization(int n,
                                int k,
                                int p,
                                int x1,
                                int y1,
                                int x2,
                                int y2) {
    bool isValid = true;

    /// Checks if both initializations had same parameters.
    if (currentGame.mapSize != DEFAULT_INITIAL_VALUE && n != currentGame.mapSize) {
        isValid = false;
    }

    if (currentGame.maxRound != DEFAULT_INITIAL_VALUE && k != currentGame.maxRound) {
        isValid = false;
    }

    if (currentGame.startingX1 != DEFAULT_INITIAL_VALUE && x1 != currentGame.startingX1) {
        isValid = false;
    }

    if (currentGame.startingY1 != DEFAULT_INITIAL_VALUE && y1 != currentGame.startingY1) {
        isValid = false;
    }

    if (currentGame.startingX2 != DEFAULT_INITIAL_VALUE && x2 != currentGame.startingX2) {
        isValid = false;
    }

    if (currentGame.startingY2 != DEFAULT_INITIAL_VALUE && y2 != currentGame.startingY2) {
        isValid = false;
    }

    /// Checks if kings were initialized on valid fields.
    if (x1 < 1 || x1 > n - 3 || y1 < 1 || y1 > n || x2 < 1 || x2 > n - 3 || y2 < 1 || y2 > n) {
        isValid = false;
    }

    /// Checks distance between kings.
    if (distMax(x1, y1, x2, y2) < MIN_INITIAL_DIST) {
        isValid = false;
    }

    /// Checks if board is out of bounds.
    if (n <= MIN_MAP_SIZE || n >= MAX_MAP_SIZE) {
        isValid = false;
    }

    /// Checks if round limit is out of bounds.
    if (k < 1 || n >= MAX_ROUND_LIMIT) {
        isValid = false;
    }

    if (p != 1 && p != 2) {
        isValid = false;
    }

    return isValid;
}

static int getPawnAdherence(pawn *currentPawn) {
    if (currentPawn == NULL) {
        return EMPTY_FIELD;
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
            return EMPTY_FIELD;
    }
}
