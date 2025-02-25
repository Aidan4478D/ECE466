#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

/*  RECYCLED HASH TABLE FROM DSA II
 *  Refurbished to C from C++
 *
 */

#define NUM_PRIMES 6
static int primes[NUM_PRIMES] = {98317, 196613, 393241, 786433, 1572869, 3145739};

typedef struct hashItem {
    char *key;
    int isOccupied;  // 0 if the slot is empty, 1 if occupied
    int isDeleted;   // 1 if the item was removed (lazy deletion), 0 otherwise
    void *pv;        // pointer associated with the key
} hashItem;

typedef struct hashTable {
    int capacity;    // capacity of the table
    int filled;      // number of active (occupied and not deleted) items
    hashItem *data;  // array of hash items
} hashTable;

// Create a hash table with an initial size.
// Returns a pointer to the hash table, or NULL on memory allocation failure.
hashTable* hashTable_create(int size);

// Destroy the hash table and free all associated memory.
void hashTable_destroy(hashTable *ht);

// Insert the specified key into the hash table and associate the pointer pv with it.
// Returns:
//   0 on success,
//   1 if the key already exists,
//   2 if rehashing fails.
int hashTable_insert(hashTable *ht, const char *key, void *pv);

// Check if the specified key exists in the hash table.
// Returns 1 if found, 0 otherwise.
bool hashTable_contains(hashTable *ht, const char *key);

// Get the pointer associated with the specified key.
// If the key is not found and b is non-NULL, *b is set to false; otherwise, *b is set to true.
// Returns the pointer if the key exists, or NULL if it does not.
//
// optional bool parameter allows the function to communicate whether the key was actually found in the hash table
// helps avoid ambiguity in cases where NULL might be a legitimate value associated with a key
void* hashTable_getPointer(hashTable *ht, const char *key, bool *b);

// Set the pointer associated with the specified key.
// Returns 0 on success, 1 if the key does not exist.
int hashTable_setPointer(hashTable *ht, const char *key, void *pv);

// Remove the item with the specified key from the hash table.
// Returns true if the key was found and removed, false otherwise.
bool hashTable_remove(hashTable *ht, const char *key);

#endif // HASH_H

