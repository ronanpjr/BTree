#include <assert.h>
#include <stdio.h>

#include "btree.h"

int main() {
    btree *tree = btree_create(3);
    btree_insert(tree, 1);
    btree_insert(tree, 2);
    btree_insert(tree, 3);

    assert(tree->root->num_keys == 3);
    assert(tree->root->keys[0] == 1);
    assert(tree->root->keys[1] == 2);
    assert(tree->root->keys[2] == 3);

    // test split
    btree_insert(tree, 4);
    assert(tree->root->num_keys == 1);
    assert(tree->root->keys[0] == 2);
    assert(tree->root->children[0]->num_keys == 1);
    assert(tree->root->children[0]->keys[0] == 1);
    assert(tree->root->children[1]->num_keys == 1);
    assert(tree->root->children[1]->keys[0] == 3);
    assert(tree->root->children[2]->num_keys == 1);
    assert(tree->root->children[2]->keys[0] == 4);

    btree_destroy(tree);

    printf("btree_insert passed\n");

    return 0;
}
