#include "btree.h"

#include <stdio.h>
#include <string.h>  // For memmove if needed

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
  int i = node->numKeys - 1;

  if (node->isLeaf) {
    // Shift keys/data to the right to make space for new key-data pair
    while (i >= 0 && node->entries[i].key > key) {
      node->entries[i + 1] = node->entries[i];
      i--;
    }

    // Insert new key-data pair
    node->entries[i + 1].key = key;
    node->entries[i + 1].data = data;
    node->numKeys++;

  } else {
    // Find the child to insert into
    while (i >= 0 && node->entries[i].key > key) {
      i--;
    }
    i++;

    // If the child is full, split it
    if (node->children[i]->numKeys == order - 1) {
      splitChild(node, i, node->children[i]);

      // After splitting, the middle key of the child moves up, so check again
      if (node->entries[i].key < key) {
        i++;
      }
    }
    insertNonFull(node->children[i], key, data, order);
  }
}

// Internal function to split a full child node
static void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
  unsigned int order = parent->children[i]->numKeys;
  BTreeNode *newChild = createNode(order, child->isLeaf);
  newChild->numKeys = (order - 1) / 2;  // New child will take half the keys

  // Move second half of child's entries to newChild
  for (unsigned int j = 0; j < newChild->numKeys; j++) {
    newChild->entries[j] = child->entries[j + (order + 1) / 2];
  }

  // If not a leaf, move child pointers too
  if (!child->isLeaf) {
    for (unsigned int j = 0; j <= newChild->numKeys; j++) {
      newChild->children[j] = child->children[j + (order + 1) / 2];
    }
  }

  // Reduce the number of keys in child
  child->numKeys = (order - 1) / 2;

  // Make space in parent for the new key
  for (int j = parent->numKeys; j >= i + 1; j--) {
    parent->children[j + 1] = parent->children[j];
  }
  parent->children[i + 1] = newChild;

  // Move the middle key of the child up to the parent
  for (int j = parent->numKeys - 1; j >= i; j--) {
    parent->entries[j + 1] = parent->entries[j];
  }
  parent->entries[i] = child->entries[(order - 1) / 2];

  // Update the parent's number of keys
  parent->numKeys++;
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

// Other functions for deletion and B* Tree operations...\

/*
// Function to print a single node
static void printNode(const BTreeNode *node) {
    if (node == NULL) return;

    // Print the keys in the node
    printf("[ ");
    for (unsigned int i = 0; i < node->numKeys; i++) {
        printf("%u ", node->entries[i].key);
    }
    printf("]");
}

// Recursive function to print the B-tree
static void printTreeRecursive(const BTreeNode *node, int level) {
    if (node == NULL) return;

    // Print the current node and its level
    printf("Level %d ", level);
    printNode(node);
    printf("\n");

    // If it's not a leaf, recursively print its children
    if (!node->isLeaf) {
        for (unsigned int i = 0; i <= node->numKeys; i++) {
            printTreeRecursive(node->children[i], level + 1);
        }
    }
} */