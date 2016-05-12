/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

#include "hashmap.h"

typedef struct gameData {
    int playerTurn;
    int currentRound;
    int mapSize;
    int maxRound;
    int startingx1;
    int startingy1;
    int startingx2;
    int startingy2;
    map_t gameMap;
} gameData;

typedef struct pawn {
    unsigned int x;
    unsigned int y;
    unsigned int lastMove;
    char symbol;
} pawn;

/**
 * Initializes a game. Needed before first INIT.
 */
void start_game();

/**
 * Frees memory. Needed after finishing game.
 */
int end_game();

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

int produce_knight(int x1,
                   int y1,
                   int x2,
                   int y2);

int produce_peasant(int x1,
                    int y1,
                    int x2,
                    int y2);

int end_turn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void print_topleft();

#endif /* ENGINE_H */

int max(int a,
        int b);

int distMax(int x1,
            int y1,
            int x2,
            int y2);

bool validInitialization(int n,
                         int k,
                         int p,
                         int x1,
                         int y1,
                         int x2,
                         int y2);