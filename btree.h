#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdbool.h>

// Enum to differentiate between the B-tree and B* Tree
typedef enum {
    BTREE_DEFAULT,
    BTREE_STAR
} BTreeType;

// Structure to hold key-data pairs
typedef struct {
    unsigned int key;
    void *data;  // Pointer to associated data
} BTreeEntry;

// B-tree structure (made opaque to users)
typedef struct BTree BTree;

// Function prototypes for creating and destroying the B-tree
BTree *createBTree(BTreeType type, unsigned int order);
void destroyBTree(BTree *tree);

// Function prototypes for default B-tree operations
void btreeInsert(BTree *tree, unsigned int key, void *data);
void btreeDelete(BTree *tree, unsigned int key);
void *btreeSearch(const BTree *tree, unsigned int key);  // Returns pointer to the associated data

// Function prototypes for B* Tree operations
void bStarTreeInsert(BTree *tree, unsigned int key, void *data);
void bStarTreeDelete(BTree *tree, unsigned int key);
void *bStarTreeSearch(const BTree *tree, unsigned int key);  // Returns pointer to the associated data

#endif // BTREE_H


