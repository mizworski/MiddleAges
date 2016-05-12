#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "engine.h"


static gameData *currentGame;

void start_game() {
    currentGame = malloc(sizeof(gameData));
    currentGame->currentRound = 0;
    currentGame->playerTurn = 0;
    currentGame->gameMap = hashmap_new();
    currentGame->mapSize = -1;
}

int end_game() {
    freeMap(currentGame->gameMap);
    free(currentGame);
    return -1;
}

void print_topleft() {

}

int init(int n,
         int k,
         int p,
         int x1,
         int y1,
         int x2,
         int y2) {
    if (!validInitialization(n, k, p, x1, y1, x2, y2)) {
        return -1;
    }
    if (p == 1) {
        char *kingKey1 = calloc(KEY_MAX_LENGTH, sizeof(char));
        char *kingKey2 = calloc(KEY_MAX_LENGTH, sizeof(char));
        pawn *king1 = newPawn(x1, y1, currentGame->currentRound,)


        snprintf(kingKey1, KEY_MAX_LENGTH, "%d:%d", x1, y1);
        snprintf(kingKey2, KEY_MAX_LENGTH, "%d:%d", x2, y2);


        hashmap_put(currentGame->gameMap, king1, 'K');
    }
    currentGame->mapSize = n;
    currentGame->maxRound = k;
    currentGame->startingx1 = x1;
    currentGame->startingy1 = y1;
    currentGame->startingx2 = x2;
    currentGame->startingy2 = y2;
}

int move(int x1,
         int y1,
         int x2,
         int y2) {

}

int produce_knight(int x1,
                   int y1,
                   int x2,
                   int y2) {

}

int produce_peasant(int x1,
                    int y1,
                    int x2,
                    int y2) {

}

int end_turn() {
    return 0;
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

bool validInitialization(int n,
                         int k,
                         int p,
                         int x1,
                         int y1,
                         int x2,
                         int y2) {
    bool valid = true;

    if (currentGame->mapSize != -1 && n != currentGame->mapSize) {
        valid = false;
    }
    if (currentGame->maxRound != -1 && k != currentGame->maxRound) {
        valid = false;
    }
    if (currentGame->startingx1 != -1 && x1 != currentGame->startingx1) {
        valid = false;
    }
    if (currentGame->startingy1 != -1 && y1 != currentGame->startingy1) {
        valid = false;
    }
    if (currentGame->startingx2 != -1 && x2 != currentGame->startingx2) {
        valid = false;
    }
    if (currentGame->startingy2 != -1 && y2 != currentGame->startingy2) {
        valid = false;
    }
    if (x1 < 1 || x1 > n || y1 < 1 || y1 > n || x2 < 1 || x2 > n || y2 < 1 || y2 > n) {
        valid = false;
    }
    if (distMax(x1, y1, x2, y2) < 8) {
        valid = false;
    }
    if (n <= 8 || n >= pow(2, 31)) {
        valid = false;
    }
    if (p != 1 && p != 2) {
        valid = false;
    }

    return valid;
}

void freeMap(map_t gameMap) {

}

pawn *newPawn(int x,
              int y,
              int currentRound,
              char symbol) {
    pawn *newPawn = malloc(sizeof(pawn));

    newPawn->x = x;
    newPawn->y = y;
    newPawn->lastMove = currentRound;
    newPawn->symbol = symbol;

    return newPawn;
}
