#include <stdio.h>

#include "btree.h"

int main() {
  BTree* tree = createBTree(BTREE_DEFAULT, 4);
  int n;
  scanf("%d", &n);
  int vetor[n];

  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
    void* data = (void*)&vetor[i];
    btreeInsert(tree, vetor[i], data);
  }

  for (int i = 0; i < n; i++) {
    void* dadoLido = btreeSearch(tree, vetor[i]);
    int* okagrvai = (int*)dadoLido;
    printf("%d ", *okagrvai);
  }

  printf("\n");

  destroyBTree(tree);
}
