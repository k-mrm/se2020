#include <stdlib.h>
#include "bptree.h"

struct node *new_node(struct node *parent) {
  struct node *n = malloc(sizeof(struct node));
  n->parent = parent;
  n->isleaf = false;
  n->nkey = 0;
  return n;
}

struct node *new_leaf(struct node *parent) {
  struct node *n = malloc(sizeof(struct node));
  n->parent = parent;
  n->isleaf = true;
  n->nkey = 0;
  return n;
}

struct node *new_root(struct node *l, int rs_key, struct node *r) {
  ;
}

struct node *find_leaf(struct node *node, int key) {
  if(node->isleaf) return node;
  int i;
  for(i = 0; i < node->nkey; i++) {
    if(key < node->key[i])
      break;
  }
  return find_leaf(node->child[i], key);
}

void insert_in_leaf(struct node *leaf, int key, struct data *data) {
  int i;
  for(i = 0; i < leaf->nkey; i++) {
    if(key < leaf->key[i])
      break;
  }
  for(int j = leaf->nkey; j > i; j--) {
    leaf->key[j] = leaf->key[j-1];
    leaf->child[j] = leaf->child[j-1];
  }
  leaf->key[i] = key;
  leaf->child[i] = (struct node *)data;
  leaf->nkey++;
}

void insert_in_temp(struct temp *t, int key, struct data *data) {
  int i;
  for(i = 0; i < t->nkey; i++) {
    if(key < t->key[i])
      break;
  }
  for(int j = t->nkey; j > i; j--) {
    t->key[j] = t->key[j-1];
    t->child[j] = t->child[j-1];
  }
  t->key[i] = key;
  t->child[i] = (struct node *)data;
  t->nkey++;
}

void erase(struct node *node) {
  for(int i = 0; i < N; i++) {
    node->child[i] = NULL;
  }
  for(int i = 0; i < N-1; i++) {
    node->key[i] = 0;
  }
  node->nkey = 0;
}

void insert_in_parent(struct node *lc, int key, struct node *rc) {
  struct node *leftp;
  struct node *rightp;

  if(lc == root) {
    root = new_root();
  }
}

void insert(int key, struct data *data) {
  struct node *leaf;
  if(root == NULL) {
    root = leaf = new_leaf(NULL);
  }
  else {
    leaf = find_leaf(root, key);
  }

  if(leaf->nkey < N-1) {
    insert_in_leaf(leaf, key, data);
  }
  else {
    /* leaf split and insert_in_parent */
    struct node *left = leaf;
    struct node *right = new_leaf(leaf->parent);
    struct temp t;
    right->child[N-1] = leaf->child[N-1]
    left->child[N-1] = right;
    cp_left2temp(&temp, left);
    insert_in_temp(&t, key, data);
    erase(left);
    cp_temp2left(t, left);
    cp_temp2right(t, right);
    int rs_key = right->key[0];
    insert_in_parent(left, rs_key, right);
  }
}

void pnode(struct node *n) {
  printf("[");
  for(int i = 0; i < n->nkey; i++) {
    if(n->isleaf) {
      printf("%d", n->key[i]);
    }
    else {
      pnode(n->child[i]);
    }
    printf(" ");
  }
  printf("]");
}

int main(int argc, char **argv) {
  if(argc != 2) {
    puts("arg error");
    return 1;
  }
  int test = argv[1][0] - '0';

  switch(test) {
    case 1:
      for(int i = 0; i < 4; i++) {
        insert(i, NULL);
      }
      pnode(root);
      return 0;
    case 2:
      for(int i = 0; i < 8; i++) {
        insert(i, NULL);
      }
      pnode(root);
      return 0;
    case 3:
      for(int i = 0; i < 10; i++) {
        insert(i, NULL);
      }
      pnode(root);
      return 0;
    case 4:
    case 5:
    case 6:
      break;
  }
}
