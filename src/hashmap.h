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

struct hashmap_map {
    unsigned int capacity;          ///< Capacity of array (number of possible hashes).
    unsigned int capacityCount;     ///< Identifies which value to take from tabCapacityValues.
    hashmap_list *hashArrayOfLists; ///< Array of lists of elements stored in map.
};

struct hashmap_list {
    unsigned int size;              ///< Length of list of elements.
    hashmap_element *element;       ///< Pointer to first element.
};

struct hashmap_element {            ///< Stores single pawn.
    pawn *currentPawn;              ///< Pawn stored.
    hashmap_element *next;          ///< Pointer to next element of
};

struct pawn {
    unsigned int x;                 ///< Column number (0-based) of pawn.
    unsigned int y;                 ///< Row number (0-based) of pawn.
    unsigned int lastMove;          ///< Number of last round when pawn moved.
    int id;                         ///< Identifier of pawn.
};

/**
 * @brief               Creates empty hashmap.
 *
 * @return              Empty hashmap.
 */
hashmap_map *hashmapCreate();

/**
 * @brief               Hashing function.
 *
 * @param capacity      Current number of different hashed.
 * @param key           Key to hash.
 * @return              Hash for key in range [0,..,capacity - 1]
 */
static unsigned int hashingFunction1(int capacity,
                                     unsigned int key);
/**
 * @brief               Hashing function.
 *
 * @param capacity      Current number of different hashed.
 * @param key           Key to hash.
 * @return              Hash for key in range [0,..,capacity - 1]
 */
static unsigned int hashingFunction2(int capacity,
                                     unsigned int key);
/**
 * @brief               Hashing function.
 *
 * @param capacity      Current number of different hashed.
 * @param x             One of keys to hash.
 * @param y             One of keys to hash.
 * @return              Hash for key in range [0,..,capacity - 1]
 */
static unsigned int hashingFunction(int capacity,
                                    unsigned int x,
                                    unsigned int y);
/**
 * @brief               Rehashes map.
 *
 * Increases number of hashes for map and generates new hash for each element in map.
 *
 * @param map           Map to rehash.
 */
static void hashmapRehash(hashmap_map *map);

/**
 * @brief               Adds pawn to map.
 *
 * @param currentPawn   Pawn that is being added.
 * @param list          Map to add to.
 */
void hashmapPut(hashmap_map *map,
                pawn *currentPawn);

/**
 * @brief               Adds pawn on list.
 *
 * @param currentPawn   Pawn that is being added.
 * @param list          List to add to.
 */
static void listAdd(pawn *currentPawn,
                    hashmap_list *list);

/**
 * @brief               Checks if pawn position is valid.
 *
 * @param currentPawn   Pawn checked.
 * @param x             Number of column.
 * @param y             Number of row.
 * @return              True if pawn is located on (x,y). False otherwise.
 */
static bool isValidPawn(pawn *currentPawn,
                        unsigned int x,
                        unsigned int y);

/**
 * @brief               Removes pawn that is located on (x,y) from map.
 *
 * @param map           Map to remove from.
 * @param x             Number of column.
 * @param y             Number of row.
 * @return              Pawn that was removed. NULL if nothing was removed from map.
 */
pawn *hashmapRemove(hashmap_map *map,
                    int x,
                    int y);

/**
 * @brief               Gets pawn that is located on (x,y), does not remove it from map.
 *
 * @param map           Map to remove from.
 * @param x             Number of column.
 * @param y             Number of row.
 * @return              Pawn if (x,y) is occupied. NULL otherwise.
 */
pawn *hashmapGet(hashmap_map *map,
                 int x,
                 int y);

/**
 * @brief               Gets pawn identifier.
 *
 * @param currentPawn   Pawn that identifier is picked.
 * @return              Identifier of pawn.
 */
int getPawnId(pawn *currentPawn);

/**
 * @brief               Gets pawn char symbol.
 *
 * @param currentPawn   Pawn that symbol is picked.
 * @return              Symbol of pawn.
 */
char getPawnSymbol(pawn *currentPawn);

/**
 * @brief               Frees map with its data.
 *
 * @param map           Map to free.
 */
void hashmapFree(hashmap_map *gameMap);

/**
 * @brief               Frees list with its elements.
 *
 * @param list          List to free.
 */
static void freeList(hashmap_list list);

/**
 * @brief               Creates new pawn.
 *
 * @param x             Column of new pawn.
 * @param y             Row of new pawn.
 * @param currentRound  Round when pawn was created.
 * @param pawnId        Identifier of new pawn.
 * @return              New pawn.
 */
pawn *newPawn(int x,
              int y,
              int currentRound,
              int pawnId);

/**
 * @brief               Mod operation, that is always greater of equals 0.
 */
static unsigned int mod(unsigned int a,
                        unsigned int b);

#endif /* HASHMAP_H */