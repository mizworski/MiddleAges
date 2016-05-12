#include "engine.h"

#include <stdlib.h>
#include <stdio.h>

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)

/* We need to keep keys and values */
typedef struct hashmap_element hashmap_element;

typedef struct hashmap_element {
    pawn *currentPawn;
    hashmap_element *next;
} hashmap_element;

typedef struct hashmap_node {
    unsigned int size;
    hashmap_element *next;
} hashmap_node;

/* A hashmap has some maximum size and current size,
 * as well as the data to hold. */
typedef struct _hashmap_map {
    unsigned int capacity;
    unsigned int capacityCount;
    unsigned int size;
    hashmap_node *data;
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

    m->data = malloc(m->capacity * sizeof(hashmap_node));
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
    hashmap_node *curr;

    /* Setup the new elements */
    hashmap_map *m = in;
    old_size = m->capacity;

    in->capacityCount++;
    m->capacity = tab_capacity_values[m->capacityCount];
    hashmap_node *temp = malloc(m->capacity * sizeof(hashmap_node));

    for (i = 0; i < m->capacity; i++) {
        m->data->size = 0;
        m->data = NULL;
    }

    /* Update the array */
    curr = m->data;
    m->data = temp;

    /* Rehash the elements */
    for (i = 0; i < old_size; i++) {
        hashmap_element *currentElement = curr[i].next;
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

int hashmap_put(hashmap_map *in,
                pawn *currentPawn) {
    int hash;
    hashmap_map *m;

    /* Find a place to put our value */
    hash = hashmap_hash(m->capacity, currentPawn->x, currentPawn->y);

    /* Set the data */
    m->data[hash].data = value;
    m->data[hash].key = key;
    m->data[hash].in_use = 1;
    m->size++;

    return MAP_OK;
}