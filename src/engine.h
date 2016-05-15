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
 * @brief       Initializes game.
 *
 * Initializes a game with size of a board, number of rounds and positions of kings.
 *
 * @param n     Board size.
 * @param k     Total number of rounds.
 * @param p     Player identifier.
 * @param x1    Column number of first player king.
 * @param y1    Row number of first player king.
 * @param x2    Column number of second player king.
 * @param y2    Row number of second player king.
 * @return      0.
 */
int init(int n,
         int k,
         int p,
         int x1,
         int y1,
         int x2,
         int y2);

/**
 * @brief       Makes a move.
 *
 * @param x1    Column number before a move.
 * @param y1    Row number before a move.
 * @param x2    Column number after a move.
 * @param y2    Row number before a move.
 * @return      0.
 */
int move(int x1,
         int y1,
         int x2,
         int y2);

/**
 * @brief               Performs action on a board.
 *
 * @param currentPawn   Pawn that makes move.
 * @param targetPawn    Pawn that occupies field on board that is targeted by pawn. NULL if field
 *                      is empty.
 * @return              Action result.
 */
static int performAction(pawn *currentPawn,
                         pawn *targetPawn);

/**
 * @brief               Checks if pawn is king.
 *
 * @param               Pawn that is checked.
 * @return              True if pawn is king. False otherwise.
 */
static bool isKing(pawn *currentPawn);

/**
 * @brief               Checks if pawn is knight.
 *
 * @param               Pawn that is checked.
 * @return              True if pawn is knight. False otherwise.
 */
static bool isKnight(pawn *currentPawn);

/**
 * @brief               Checks if pawn is peasant.
 *
 * @param               Pawn that is checked.
 * @return              True if pawn is peasant. False otherwise.
 */
static bool isPeasant(pawn *currentPawn);

/**
 * @brief               Produces knights.
 *
 * @param x1            Column number of peasant.
 * @param y1            Row number of peasant.
 * @param x2            Column number of new knight.
 * @param y2            Row number of new knight.
 * @return              GAME_OK if unit was created. ERROR otherwise.
 */
int produceKnight(int x1,
                  int y1,
                  int x2,
                  int y2);

/**
 * @brief               Produces peasant.
 *
 * @param x1            Column number of peasant.
 * @param y1            Row number of peasant.
 * @param x2            Column number of new peasant.
 * @param y2            Row number of new peasant.
 * @return              GAME_OK if unit was created. ERROR otherwise.
 */
int producePeasant(int x1,
                   int y1,
                   int x2,
                   int y2);

/**
 * @brief               Produces specified unit.
 *
 * @param x1            Column number of peasant.
 * @param y1            Row number of peasant.
 * @param x2            Column number of new knight.
 * @param y2            Row number of new knight.
 * @param unitId        Identifier of new unit
 * @return              GAME_OK if unit was created. ERROR otherwise.
 */
static int produceUnit(int x1,
                       int y1,
                       int x2,
                       int y2,
                       int unitId);

/**
 * @brief               Ends turn.
 *
 * @return              ERROR if game was not initialized before calling.
 *                      EXCEEDED_ROUND_LIMIT if round limit was exceeded.
 *                      GAME_OK otherwise.
 */
int endTurn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void printTopLeft();

/**
 * @brief               Maximum of two integers.
 */
static int max(int a,
               int b);

/**
 * @brief               Distance in max metrics.
 */
static int distMax(int x1,
                   int y1,
                   int x2,
                   int y2);

/**
 * @brief               Checks if field is valid.
 *
 * @return              False if x or y is out of map bounds. True otherwise.
 */
static bool isValidField(int mapSize,
                         int x,
                         int y);

/**
 * @brief               Checks if initialization was valid.
 *
 * @return              True if initialization was valid. False otherwise.
 */
static bool validInitialization(int n,
                                int k,
                                int p,
                                int x1,
                                int y1,
                                int x2,
                                int y2);

/**
 * @brief               Gets pawn adherence.
 *
 * @param currentPawn   Pawn to check.
 * @return              Identifier of player that possess pawn.
 */
static int getPawnAdherence(pawn *currentPawn);

#endif /* ENGINE_H */