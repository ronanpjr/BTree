#include <assert.h>
#include <stdio.h>

#include "btree.h"

int main() {
    btree *tree = btree_create(3);

    assert(tree->root->num_keys == 0);
    assert(tree->root->is_leaf == 1);
    assert(tree->t == 3);

    btree_destroy(tree);

    printf("btree_create passed\n");

    return 0;
}
