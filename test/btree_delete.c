#include <assert.h>
#include <stdio.h>

#include "btree.h"

int main() {
    btree *tree = btree_create(3);
    btree_insert(tree, 1);
    btree_insert(tree, 2);
    btree_insert(tree, 3);

    btree_delete(tree, 2);

    assert(tree->root->num_keys == 2);
    assert(tree->root->keys[0] == 1);
    assert(tree->root->keys[1] == 3);

    btree_destroy(tree);

    printf("btree_delete passed\n");

    return 0;
}
