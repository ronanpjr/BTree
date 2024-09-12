#include "btree.h"
#include <stdio.h>

void main() {

    BTree *tree = createBTree(BTREE_DEFAULT, 4);
    int x;
    scanf("%d" , &x);
    void *data  = (void*) x;
    btreeInsert(tree, x, data);

    void* dadoLido = btreeSearch(tree, x);
    int* okagrvai = (int*) dadoLido;
    printf("%d", *okagrvai);
}