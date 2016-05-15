/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

#include "hashmap.h"

#define END_TURN_RETURN_VALUE   -2
#define ERROR                   -1
#define GAME_OK                 0
#define PLAYER_A_WON            1
#define PLAYER_B_WON            2
#define DRAW                    3
#define EXCEEDED_ROUND_LIMIT    4

typedef struct gameData {           ///< Information about current game.
    int playerTurn;                 ///< Which player turn currently is.
    unsigned int currentRound;      ///< Number of current round.
    int mapSize;                    ///< Length of map side.
    int maxRound;                   ///< Max number of rounds.
    int startingX1;                 ///< Initial column number of first player's king.
    int startingY1;                 ///< Initial row number of first player's king.
    int startingX2;                 ///< Initial column number of second player's king.
    int startingY2;                 ///< Initial row number of second player's king.
    hashmap_map *gameMap;           ///< Map that stores game board.
    bool hasPlayerAInitialized;     ///< Checks if first player initialized already.
    bool hasPlayerBInitialized;     ///< Checks if second player initialized already.
} gameData;

/**
 * Initializes a game. Needed before first INIT.
 */
void startGame();

/**
 * Frees memory. Needed after finishing game.
 */
void endGame();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 */
int init(int n,
         int k,
         int p,
         int x1,
         int y1,
         int x2,
         int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 0.
 */
int move(int x1,
         int y1,
         int x2,
         int y2);

static int performAction(pawn *currentPawn,
                         pawn *targetPawn);

static bool isKing(pawn *currentPawn);

static bool isKnight(pawn *currentPawn);

static bool isPeasant(pawn *currentPawn);

int produceKnight(int x1,
                  int y1,
                  int x2,
                  int y2);

int producePeasant(int x1,
                   int y1,
                   int x2,
                   int y2);

static int produceUnit(int x1,
                       int y1,
                       int x2,
                       int y2,
                       int unitId);

int endTurn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void printTopLeft();

static int max(int a,
               int b);

static int distMax(int x1,
                   int y1,
                   int x2,
                   int y2);

static bool isValidField(int mapSize,
                         int x,
                         int y);

static bool validInitialization(int n,
                                int k,
                                int p,
                                int x1,
                                int y1,
                                int x2,
                                int y2);


static int getPawnAdherence(pawn *currentPawn);


#endif /* ENGINE_H */