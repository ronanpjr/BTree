#include <stdio.h>
#include <string.h>  // For memmove if needed

#include "btree.h"

// Internal structure definitions that are private to the implementation file

// Node structure for B-tree and B* Tree
typedef struct BTreeNode {
  BTreeEntry *entries;          // Array to hold key-data pairs
  struct BTreeNode **children;  // Array of child pointers
  unsigned int numKeys;         // Number of keys currently in the node
  bool isLeaf;                  // Boolean: is this node a leaf?
} BTreeNode;

// Full definition of the BTree structure (hidden from the user)
struct BTree {
  BTreeNode *root;     // Pointer to the root node
  unsigned int order;  // Order of the B-tree
  BTreeType type;      // Type: either BTREE_DEFAULT or BTREE_STAR
};

// Internal function prototypes (not visible to the user)
static BTreeNode *createNode(unsigned int order, bool isLeaf);
static void freeNode(BTreeNode *node);
static void splitChild(BTreeNode *parent, int i, BTreeNode *child);
static void insertNonFull(BTreeNode *node, unsigned int key, void *data,
                          unsigned int order);
static void *searchNode(const BTreeNode *node, unsigned int key);

// Function to create a new B-tree
BTree *createBTree(BTreeType type, unsigned int order) {
  BTree *tree = (BTree *)malloc(sizeof(BTree));
  if (!tree) {
    return NULL;  // Handle allocation failure
  }
  tree->root = createNode(order, true);  // Create the root node as a leaf
  tree->order = order;
  tree->type = type;
  return tree;
}

// Function to destroy a B-tree and free all allocated memory
void destroyBTree(BTree *tree) {
  if (!tree) return;

  freeNode(tree->root);  // Recursively free nodes
  free(tree);
}

// Internal function to recursively free a node and its children
static void freeNode(BTreeNode *node) {
  if (node == NULL) return;

  // Recursively free child nodes
  if (!node->isLeaf) {
    for (unsigned int i = 0; i <= node->numKeys; i++) {
      freeNode(node->children[i]);
    }
  }

  // Free the entries and children
  free(node->entries);
  free(node->children);

  // Free the node itself
  free(node);
}

// Internal function to create a new node
static BTreeNode *createNode(unsigned int order, bool isLeaf) {
  BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
  if (!node) return NULL;

  node->entries = (BTreeEntry *)malloc(sizeof(BTreeEntry) *
                                       (order - 1));  // Allocate for keys/data
  node->children = (BTreeNode **)malloc(sizeof(BTreeNode *) *
                                        order);  // Allocate for children
  node->numKeys = 0;
  node->isLeaf = isLeaf;

  return node;
}

// Function to insert a key and its associated data into the B-tree
void btreeInsert(BTree *tree, unsigned int key, void *data) {
  if (tree->root->numKeys == tree->order - 1) {
    // Root is full, so split
    BTreeNode *newRoot = createNode(tree->order, false);
    newRoot->children[0] = tree->root;
    splitChild(newRoot, 0, tree->root);
    tree->root = newRoot;
  }
  insertNonFull(tree->root, key, data, tree->order);
}

// Internal function to insert into a non-full node
static void insertNonFull(BTreeNode *node, unsigned int key, void *data,
                          unsigned int order) {
  // Implementation of inserting into a non-full node (key-data pair)
  // Adjust logic to handle BTreeEntry for key-data pairs
}

// Internal function to split a full child node
static void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
  // Implementation of splitting a full child node
}

// Function to search for a key in the B-tree and return the associated data
void *btreeSearch(const BTree *tree, unsigned int key) {
  return searchNode(tree->root, key);
}

// Internal function to search for a key in a node
static void *searchNode(const BTreeNode *node, unsigned int key) {
  // Search logic to return the associated data for a key
  unsigned int i = 0;
  for (i; i < node->numKeys; i++) {
    if (node->entries[i].key == key) {
      return node->entries[i].data;
    }
  }
  
  // If not found and it's a leaf, return NULL
  if (node->isLeaf) {
    return NULL;
  }
  // Otherwise, recursively search the appropriate child
  
  return searchNode(node->children[i],
                    key);  // Adjust child traversal based on key comparison
}

// Other functions for deletion and B* Tree operations...