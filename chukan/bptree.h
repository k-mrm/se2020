#include <stdio.h>
#include <stdbool.h>

#define N 4

struct data {
  int key;
  int val;
};

struct node {
  bool isleaf;
  struct node *child[N];
  int key[N-1];
  int nkey;
  struct node *parent;
};

struct temp {
  struct node *child[N+1];
  int key[N];
  int nkey;
};

struct node *root = NULL;
