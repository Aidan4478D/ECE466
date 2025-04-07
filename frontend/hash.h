#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

/*  RECYCLED HASH TABLE FROM DSA II
 *  Refurbished to C from C++
*/

#define NUM_PRIMES 6

typedef struct hash_item {
    char *key;
    int isOccupied;  // 0 if the slot is empty, 1 if occupied
    int isDeleted;   // 1 if the item was removed (lazy deletion), 0 otherwise
    void *pv;        // pointer associated with the key
} hash_item;

typedef struct hash_table {
    int capacity;
    int filled;
    hash_item *data;
} ht_t;

// Create a hash table with an initial size.
// Returns a pointer to the hash table, or NULL on memory allocation failure.
ht_t* ht_create(int size);

// Destroy the hash table and free all associated memory.
void ht_destroy(ht_t *ht);

// Insert the specified key into the hash table and associate the pointer pv with it.
// Returns:
//   0 on success,
//   1 if the key already exists,
//   2 if rehashing fails.
int ht_insert(ht_t *ht, const char *key, void *pv);

// Check if the specified key exists in the hash table.
// Returns 1 if found, 0 otherwise.
bool ht_contains(ht_t *ht, const char *key);

// Get the pointer associated with the specified key.
// If the key is not found and b is non-NULL, *b is set to false; otherwise, *b is set to true.
// Returns the pointer if the key exists, or NULL if it does not.
//
// optional bool parameter allows the function to communicate whether the key was actually found in the hash table
// helps avoid ambiguity in cases where NULL might be a legitimate value associated with a key
void* ht_get_pointer(ht_t *ht, const char *key, bool *b);

// Set the pointer associated with the specified key.
// Returns 0 on success, 1 if the key does not exist.
int ht_set_pointer(ht_t *ht, const char *key, void *pv);

// Remove the item with the specified key from the hash table.
// Returns true if the key was found and removed, false otherwise.
bool ht_remove(ht_t *ht, const char *key);

#endif // HASH_H

