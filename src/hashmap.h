/** @file
   Interface of data structure.

*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

#define KING_PLAYER_A_ID        0
#define KNIGHT_PLAYER_A_ID      1
#define PEASANT_PLAYER_A_ID     2
#define KING_PLAYER_B_ID        10
#define KNIGHT_PLAYER_B_ID      11
#define PEASANT_PLAYER_B_ID     12
#define EMPTY_SPACE_ID          20

#define KING_PLAYER_A_CHAR      'K'
#define KNIGHT_PLAYER_A_CHAR    'R'
#define PEASANT_PLAYER_A_CHAR   'C'
#define KING_PLAYER_B_CHAR      'k'
#define KNIGHT_PLAYER_B_CHAR    'r'
#define PEASANT_PLAYER_B_CHAR   'c'
#define EMPTY_SPACE_CHAR        '.'

typedef struct hashmap_map hashmap_map;
typedef struct hashmap_list hashmap_list;
typedef struct hashmap_element hashmap_element;
typedef struct pawn pawn;

/* A hashmap has some maximum size and current size,
 * as well as the hashArrayOfLists to hold. */
struct hashmap_map {
    unsigned int capacity;
    unsigned int capacityCount;
    unsigned int size;
    hashmap_list *hashArrayOfLists;
};

struct hashmap_list {
    unsigned int size;
    hashmap_element *element;
};

struct hashmap_element {
    pawn *currentPawn;
    hashmap_element *next;
};

struct pawn {
    unsigned int x;
    unsigned int y;
    unsigned int lastMove;
    int id;
};

/*
 * Return an empty hashmap, or NULL on failure.
 */
hashmap_map *hashmapCreate();

unsigned int hashingFunction1(int capacity,
                              unsigned int key);

unsigned int hashingFunction2(int capacity,
                              unsigned int key);

unsigned int hashingFunction(int capacity,
                             unsigned int x,
                             unsigned int y);

void hashmapRehash(hashmap_map *in);

void listAdd(pawn *currentPawn,
             hashmap_list *list);

void hashmapPut(hashmap_map *m,
                pawn *currentPawn);

bool isValidPawn(pawn *currentPawn,
                 unsigned int x,
                 unsigned int y);

pawn *hashmapRemove(hashmap_map *m,
                    int x,
                    int y);

pawn *hashmapGet(hashmap_map *m,
                 int x,
                 int y);

        int getPawnId(pawn *currentPawn);

char getPawnSymbol(pawn *currentPawn);

void listRemove(int x,
                int y,
                hashmap_list list);

/*void hashmapRemove(hashmap_map *m,
                   int x,
                   int y);*/

/* Return the length of the hashmap */
int hashmapLength(hashmap_map *m);

void hashmapFree(hashmap_map *gameMap);

void freeList(hashmap_list list);

pawn *newPawn(int x,
              int y,
              int currentRound,
              int pawnId);

#endif /* HASHMAP_H */