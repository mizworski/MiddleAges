#include "engine.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_CHAIN_LENGTH 64
#define MAX_CAPACITY 0x7fffffff

static unsigned int tab_capacity_values[] = {
        0x101, 0x209, 0x407, 0x805,
        0x1003, 0x2011, 0x401b, 0x8003,
        0x10001, 0x2001d, 0x40003, 0x80015,
        0x100007, 0x200011, 0x40000f, 0x800009,
        0x100002b, 0x2000023, 0x400000f, 0x800001d,
        0x10000003, 0x2000000b, 0x40000003, 0x7fffffff
};

unsigned int mod(unsigned int a,
                 unsigned int b) {
    return a % b >= 0 ? a % b : a % b + b;
}

/*
 * Return an empty hashmap, or NULL on failure.
 */
hashmap_map *hashmapCreate() {
    hashmap_map *m = malloc(sizeof(hashmap_map));
    if (!m) goto err;

    m->capacityCount = 0;
    m->capacity = tab_capacity_values[m->capacityCount];

    m->hashArrayOfLists = malloc(m->capacity * sizeof(hashmap_list));
    if (!m->hashArrayOfLists) goto err;
    for (int i = 0; i < m->capacity; i++) {
        m->hashArrayOfLists[i].size = 0;
        m->hashArrayOfLists[i].element = NULL;
    }

    return m;
    err:
    if (m)
        hashmapFree(m);
    return NULL;
}

unsigned int hashingFunction1(int capacity,
                              unsigned int key) {
    key = (key + 0x7ed55d16) + (key << 12);
    key = (key ^ 0xc761c23c) ^ (key >> 19);
    key = (key + 0x165667b1) + (key << 5);
    key = (key + 0xd3a2646c) ^ (key << 9);
    key = (key + 0xfd7046c5) + (key << 3);
    key = (key ^ 0xb55a4f09) ^ (key >> 16);

    return mod(key, (unsigned int) capacity);
}

unsigned int hashingFunction2(int capacity,
                              unsigned int key) {
    int c2 = 0x27d4eb2d; // a prime or an odd constant

    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);

    return mod(key, (unsigned int) capacity);
}

unsigned int hashingFunction(int capacity,
                             unsigned int x,
                             unsigned int y) {
    return mod(hashingFunction1(capacity, x) + y * hashingFunction2(capacity, x),
               (unsigned int) capacity);
}

void hashmapRehash(hashmap_map *map) {
    int old_size;
    hashmap_list *curr;
    old_size = map->capacity;

    map->capacityCount++;
    map->capacity = tab_capacity_values[map->capacityCount];
    hashmap_list *temp = malloc(map->capacity * sizeof(hashmap_list));

    for (int i = 0; i < map->capacity; i++) {
        temp[i].size = 0;
        temp[i].element = NULL;
    }

    /* Update the array */
    curr = map->hashArrayOfLists;
    map->hashArrayOfLists = temp;

    /* Rehash the elements */
    for (int i = 0; i < old_size; i++) {
        hashmap_element *tempElement;
        hashmap_element *currentElement = curr[i].element;
        while (currentElement != NULL) {
            tempElement = currentElement;
            currentElement = currentElement->next;
            hashmapPut(map, tempElement->currentPawn);
            free(tempElement);
        }
    }

    free(curr);
}

void hashmapPut(hashmap_map *m,
                pawn *currentPawn) {
    int hash;
    hashmap_list *list;

    /* Find a place to put our value */
    hash = hashingFunction(m->capacity, currentPawn->x, currentPawn->y);

    /* Set the hashArrayOfLists */
    list = &m->hashArrayOfLists[hash];

    listAdd(currentPawn, list);

    if (list->size > MAX_CHAIN_LENGTH && m->capacity < MAX_CAPACITY) {
        hashmapRehash(m);
    }
}

pawn *hashmapRemove(hashmap_map *m,
                    int x,
                    int y) {
    int hash;
    hashmap_list *list;
    hashmap_element *currentElement;
    hashmap_element *previousElement;

    x--;
    y--;

    /* Find hashArrayOfLists location */
    hash = hashingFunction(m->capacity, (unsigned int) x, (unsigned int) y);
    list = &m->hashArrayOfLists[hash];

    if (list->element == NULL) {
        return NULL;
    } else {
        currentElement = list->element;
        previousElement = NULL;
        while (currentElement->next != NULL && !isValidPawn(currentElement->currentPawn,
                                                            (unsigned int) x,
                                                            (unsigned int) y)) {
            previousElement = currentElement;
            currentElement = currentElement->next;
        }
        if (currentElement == NULL) {
            return NULL;
        } else {
            if (isValidPawn(currentElement->currentPawn, (unsigned int) x, (unsigned int) y)) {
                pawn *currentPawn = currentElement->currentPawn;
                if (previousElement == NULL) {
                    list->element = currentElement->next;
                } else {
                    previousElement->next = currentElement->next;

                }
                free(currentElement);
                return currentPawn;
            } else {
                return NULL;
            }
        }
    }
    return NULL;
}

pawn *hashmapGet(hashmap_map *m,
                 int x,
                 int y) {
    int hash;
    hashmap_list *list;
    hashmap_element *currentElement;

    x--;
    y--;

    /* Find hashArrayOfLists location */
    hash = hashingFunction(m->capacity, (unsigned int) x, (unsigned int) y);
    list = &m->hashArrayOfLists[hash];

    if (list->element == NULL) {
        return NULL;
    } else {
        currentElement = list->element;
        while (currentElement->next != NULL && !isValidPawn(currentElement->currentPawn,
                // TODO problem
                                                            (unsigned int) x,
                                                            (unsigned int) y)) {
            currentElement = currentElement->next;
        }
        if (currentElement == NULL) {
            return NULL;
        } else {
            if (isValidPawn(currentElement->currentPawn, (unsigned int) x, (unsigned int) y)) {
                pawn *currentPawn = currentElement->currentPawn;
                return currentPawn;

            } else {
                return NULL;
            }
        }
    }
}

int getPawnId(pawn *currentPawn) {

    return currentPawn == NULL ? EMPTY_SPACE_ID : currentPawn->id;
}

char getPawnSymbol(pawn *currentPawn) {
    int pawnId;
    char symbol;

    pawnId = getPawnId(currentPawn);

    switch (pawnId) {
        case KING_PLAYER_A_ID:
            symbol = KING_PLAYER_A_CHAR;
            break;
        case KING_PLAYER_B_ID:
            symbol = KING_PLAYER_B_CHAR;
            break;
        case KNIGHT_PLAYER_A_ID:
            symbol = KNIGHT_PLAYER_A_CHAR;
            break;
        case KNIGHT_PLAYER_B_ID:
            symbol = KNIGHT_PLAYER_B_CHAR;
            break;
        case PEASANT_PLAYER_A_ID:
            symbol = PEASANT_PLAYER_A_CHAR;
            break;
        case PEASANT_PLAYER_B_ID:
            symbol = PEASANT_PLAYER_B_CHAR;
            break;
        case EMPTY_SPACE_ID:
            symbol = EMPTY_SPACE_CHAR;
            break;
        default:
            symbol = EMPTY_SPACE_CHAR;
    }

    return symbol;
}

bool isValidPawn(pawn *currentPawn,
                 unsigned int x,
                 unsigned int y) {
    return currentPawn != NULL ? currentPawn->x == x ? currentPawn->y == y ? true : false : false :
           false;
}


void listAdd(pawn *currentPawn,
             hashmap_list *list) {
    hashmap_element *currentElement;
    hashmap_element *newElement = malloc(sizeof(hashmap_element));
    newElement->currentPawn = currentPawn;
    newElement->next = NULL;

    if (list->element == NULL) {
        list->element = newElement;
    } else {
        currentElement = list->element;
        while (currentElement->next != NULL) {
            currentElement = currentElement->next;
        }
        currentElement->next = newElement;

    }
    list->size++;
}

void hashmapFree(hashmap_map *gameMap) {
    for (unsigned int i = 0; i < gameMap->capacity; i++) {
        freeList(gameMap->hashArrayOfLists[i]);
    }
    free(gameMap->hashArrayOfLists);
    free(gameMap);
}

void freeList(hashmap_list list) {
    hashmap_element *nextElement = list.element;

    while (nextElement != NULL) {
        hashmap_element *tempElement = nextElement;
        nextElement = nextElement->next;
        free(tempElement->currentPawn);
        free(tempElement);
    }
}

pawn *newPawn(int x,
              int y,
              int currentRound,
              int pawnId) {
    x--;
    y--;

    pawn *newPawn = malloc(sizeof(pawn));

    newPawn->x = (unsigned int) x;
    newPawn->y = (unsigned int) y;
    newPawn->lastMove = (unsigned int) currentRound;
    newPawn->id = pawnId;

    return newPawn;
}
