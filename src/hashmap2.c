#include "engine.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_CHAIN_LENGTH 64
#define MAX_CAPACITY 0x7fffffff

/* We need to keep keys and values */
typedef struct hashmap_element hashmap_element;

typedef struct hashmap_element {
    pawn *currentPawn;
    hashmap_element *next;
} hashmap_element;

typedef struct hashmap_list {
    unsigned int size;
    hashmap_element *element;
} hashmap_list;

/* A hashmap has some maximum size and current size,
 * as well as the data to hold. */
typedef struct _hashmap_map {
    unsigned int capacity;
    unsigned int capacityCount;
    unsigned int size;
    hashmap_list *data;
} hashmap_map;

static unsigned int tab_capacity_values[] = {
        0x101, 0x209, 0x407, 0x805,
        0x1003, 0x2011, 0x401b, 0x8003,
        0x10001, 0x2001d, 0x40003, 0x80015,
        0x100007, 0x200011, 0x40000f, 0x800009,
        0x100002b, 0x2000023, 0x400000f, 0x800001d,
        0x10000003, 0x2000000b, 0x40000003, 0x7fffffff
};


/*
 * Return an empty hashmap, or NULL on failure.
 */
hashmap_map *hashmap_new() {
    hashmap_map *m = malloc(sizeof(hashmap_map));
    if (!m) goto err;

    m->capacityCount = 0;
    m->capacity = tab_capacity_values[m->capacityCount];
    m->size = 0;

    m->data = malloc(m->capacity * sizeof(hashmap_list));
    if (!m->data) goto err;
    for (int i = 0; i < m->capacity; i++) {
        m->data->size = 0;
        m->data = NULL;
    }

    return m;
    err:
    if (m)
        hashmap_free(m);
    return NULL;
}

unsigned int hashmap_hash_1(int capacity,
                            unsigned int key) {


    /* Robert Jenkins' 32 bit Mix Function */
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key >> 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);

    /* Knuth's Multiplicative Method */
    key = (key >> 3) * 2654435761;

    return key % capacity;
}

unsigned int hashmap_hash_2(int capacity,
                            unsigned int key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key);

    return key % capacity;
}

unsigned int hashmap_hash(int capacity,
                          unsigned int x,
                          unsigned int y) {
    return hashmap_hash_1(capacity, x) + y * hashmap_hash_2(capacity, x) % capacity;
}

int hashmap_rehash(hashmap_map *in) {
    int i;
    int old_size;
    hashmap_list *curr;

    /* Setup the new elements */
    hashmap_map *m = in;
    old_size = m->capacity;

    in->capacityCount++;
    m->capacity = tab_capacity_values[m->capacityCount];
    hashmap_list *temp = malloc(m->capacity * sizeof(hashmap_list));

    for (i = 0; i < m->capacity; i++) {
        m->data->size = 0;
        m->data = NULL;
    }

    /* Update the array */
    curr = m->data;
    m->data = temp;

    /* Rehash the elements */
    for (i = 0; i < old_size; i++) {
        hashmap_element *currentElement = curr[i].element;
        while (currentElement != NULL) {
            hashmap_element *tempElement = currentElement;
            currentElement = currentElement->next;
            hashmap_put(in, currentElement->currentPawn);
            free(tempElement);
        }
    }

    free(curr);

    return MAP_OK;
}

void list_add(pawn *currentPawn,
              hashmap_list list) {
    hashmap_element *currentElement;
    hashmap_element *newElement = malloc(sizeof(hashmap_element));
    newElement->currentPawn = currentPawn;
    newElement->next = NULL;

    if (list.element == NULL) {
        list.element = newElement;
    } else {
        currentElement = list.element;
        while (currentElement->next != NULL) {
            currentElement = currentElement->next;
        }
        currentElement->next = newElement;

    }
    list.size++;
}

int hashmap_put(hashmap_map *m,
                pawn *currentPawn) {
    int hash;
    hashmap_list list;

    /* Find a place to put our value */
    hash = hashmap_hash(m->capacity, currentPawn->x, currentPawn->y);

    /* Set the data */
    list = m->data[hash];

    list_add(currentPawn, list);

    if (list.size > MAX_CHAIN_LENGTH && m->capacity < MAX_CAPACITY) {
        hashmap_rehash(m);
    }

    return MAP_OK;
}

bool isValidPawn(pawn *currentPawn,
                 unsigned int x,
                 unsigned int y) {
    return currentPawn != NULL ? currentPawn->x == x ? currentPawn->y == y ? true : false : false :
           false;
}

pawn *hashmap_get(hashmap_map *m,
                  int x,
                  int y) {
    int hash;
    hashmap_list list;
    hashmap_element *currentElement;

    /* Find data location */
    hash = hashmap_hash(m->capacity, (unsigned) x, (unsigned) y);
    list = m->data[hash];

    if (list.element == NULL) {
        return NULL;
    } else {
        currentElement = list.element;
        while (currentElement->next != NULL && !isValidPawn(currentElement->currentPawn,
                                                            (unsigned) x,
                                                            (unsigned) y)) {
            currentElement = currentElement->next;
        }
        if (isValidPawn(currentElement->currentPawn, (unsigned) x, (unsigned) y)) {
            return currentElement->currentPawn;
        }
        return NULL;
    }
}

int hashmap_remove(hashmap_map *m,
                   int x,
                   int y) {
    int i;
    int key;

    /* Find key */
    key = hashmap_hash(m->capacity, currentPawn->x, currentPawn->y);

    /* Linear probing, if necessary */
    for (i = 0; i < MAX_CHAIN_LENGTH; i++) {

        int in_use = m->data[key].in_use;
        if (in_use == 1) {
            if (strcmp(m->data[key].key, key) == 0) {
                /* Blank out the fields */
                m->data[key].in_use = 0;
                m->data[key].data = NULL;
                m->data[key].key = NULL;

                /* Reduce the size */
                m->size--;
                return MAP_OK;
            }
        }
        key = (key + 1) % m->capacity;
    }

    /* Data not found */
    return MAP_MISSING;
}

/* Deallocate the hashmap */
void hashmap_free(hashmap_map *m) {
    free(m->data);
    free(m);
}

/* Return the length of the hashmap */
int hashmap_length(hashmap_map *m) {
    if (m != NULL) return m->size;
    else return 0;
}