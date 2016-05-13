/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

#include "hashmap.h"

#define ERROR           -1
#define GAME_OK         0
#define PLAYER_A_WON    1
#define PLAYER_B_WON    2
#define DRAW            3

typedef struct gameData {
    int playerTurn;
    int currentRound;
    int mapSize;
    int maxRound;
    int startingx1;
    int startingy1;
    int startingx2;
    int startingy2;
    hashmap_map *gameMap;
    bool playerAinit;
    bool playerBinit;
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

int performAction(pawn *currentPawn,
                  pawn *targetPawn);

int produceKnight(int x1,
                  int y1,
                  int x2,
                  int y2);

int producePeasant(int x1,
                   int y1,
                   int x2,
                   int y2);

int produceUnit(int x1,
                int y1,
                int x2,
                int y2,
                int unitId);

int endTurn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void printTopLeft();

int max(int a,
        int b);

int distMax(int x1,
            int y1,
            int x2,
            int y2);

bool isValidField(int mapSize,
                  int x,
                  int y);

bool validInitialization(int n,
                         int k,
                         int p,
                         int x1,
                         int y1,
                         int x2,
                         int y2);


bool isValidOperation(gameData *currentGame,
                      int x1,
                      int y1,
                      int x2,
                      int y2,
                      int operationType);

int getPawnAdherence(pawn *currentPawn);


#endif /* ENGINE_H */