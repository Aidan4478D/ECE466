#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int rehash(ht_t *ht);

// return a prime number at least as large as size.
int get_prime(int size) {
    static int primes[NUM_PRIMES] = {98317, 196613, 393241, 786433, 1572869, 3145739};
    if (size < primes[0]) return primes[0];

    for (int i = 0; i < NUM_PRIMES - 1; i++) {
        if (size > primes[i] && size < primes[i + 1])
            return primes[i + 1];
    }
    return primes[NUM_PRIMES - 1];
}

/*  hash function from 
 *  Chapter 5.2, Figure 5.4
 *  "Data Structures and Algorithm Analysis in C++", 4th Edition by Mark Allen Weis 
 *  aka this is ripped from the DSA II textbook
 */
int hash_func(ht_t *ht, const char *key) {
    unsigned int hashVal = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashVal = 37 * hashVal + (unsigned char) key[i];
    }
    return hashVal % ht->capacity;
}

// searches for the key and returns its index if found, -1 if not
int find_pos(ht_t *ht, const char *key) {
    int hash_val = hash_func(ht, key);
    int iterations = 0;
    while (ht->data[hash_val].isOccupied) {
        if (!ht->data[hash_val].isDeleted && ht->data[hash_val].key &&
            strcmp(ht->data[hash_val].key, key) == 0) {
            return hash_val;
        }
        hash_val = (hash_val + 1) % ht->capacity;
        iterations++;
        if (iterations > ht->capacity)
            break;
    }
    return -1;
}

// create hash table
ht_t *ht_create(int size) {
    ht_t *ht = malloc(sizeof(ht_t));
    if (!ht)
        return NULL;
    ht->filled = 0;
    ht->capacity = get_prime(size);
    ht->data = calloc(ht->capacity, sizeof(hash_item));
    if (!ht->data) {
        free(ht);
        return NULL;
    }
    return ht;
}

// destroy the hash table and free all associated memory
void ht_destroy(ht_t *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->data[i].isOccupied && !ht->data[i].isDeleted) {
            free(ht->data[i].key);
        }
    }
    free(ht->data);
    free(ht);
}

// insert a key into the hash table.
int ht_insert(ht_t* ht, const char* key, void* pv) {
    // return 1 if key already exists.
    if (ht_contains(ht, key)) return 1;
    
    // rehash if table is at least half full.
    if (ht->filled >= ht->capacity / 2) {
        if (!rehash(ht)) return 2;
    }
    
    int hash_val = hash_func(ht, key);
    int iterations = 0;
    while (ht->data[hash_val].isOccupied && !ht->data[hash_val].isDeleted) {
        hash_val = (hash_val + 1) % ht->capacity;
        iterations++;
        if (iterations > ht->capacity) return -1;
    }
    
    char *dupKey = strdup(key);
    if (!dupKey)
        return -1;
    
    ht->data[hash_val].key = dupKey;
    ht->data[hash_val].pv = pv;
    ht->data[hash_val].isOccupied = 1;
    ht->data[hash_val].isDeleted = 0;
    ht->filled++;
    return 0;
}

// check if the key exists in the table
bool ht_contains(ht_t *ht, const char *key) {
    return (find_pos(ht, key) != -1);
}

// get the pointer associated with the key
void *ht_getPointer(ht_t *ht, const char *key, bool *b) {
    int pos = find_pos(ht, key);
    if (pos == -1) {
        if (b) *b = false;
        return NULL;
    }
    if (b) *b = true;
    return ht->data[pos].pv;
}

// set the pointer associated with the key
int ht_setPointer(ht_t *ht, const char *key, void *pv) {
    int pos = find_pos(ht, key);
    if (pos != -1) {
        ht->data[pos].pv = pv;
        return 0;
    }
    return 1;
}

// remove the key from the table (lazy deletion)
bool ht_remove(ht_t *ht, const char *key) {
    int pos = find_pos(ht, key);
    if (pos != -1) {
        free(ht->data[pos].key);
        ht->data[pos].key = NULL;
        ht->data[pos].isDeleted = 1;
        return true;
    }
    return false;
}

// Rehash: create a new data array with increased capacity and reinsert all active keys.
int rehash(ht_t *ht) {
    int old_capacity = ht->capacity;
    hash_item *old_data = ht->data;
    
    int new_capacity = get_prime(ht->capacity * 2);
    hash_item *new_data = calloc(new_capacity, sizeof(hash_item));
    if (!new_data)
        return 0;
    
    // update hash table structure.
    ht->capacity = new_capacity;
    ht->filled = 0;
    ht->data = new_data;
    
    // reinsert each active item from the old table.
    for (int i = 0; i < old_capacity; i++) {
        if (old_data[i].isOccupied && !old_data[i].isDeleted) {
            int ret = ht_insert(ht, old_data[i].key, old_data[i].pv);
            free(old_data[i].key);
            if (ret != 0) {
                free(old_data);
                return 0;
            }
        }
    }
    free(old_data);
    return 1;
}

