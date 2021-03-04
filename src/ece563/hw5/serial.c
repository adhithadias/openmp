#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

struct node {
  double val;
  struct node* l;
  struct node* r;
} nodeT;

#define MAXLEVEL 18

struct node* build(int level) {

  if (level < MAXLEVEL) {
    struct node* p = (struct node*) malloc(sizeof(nodeT));
    p->val = (double)rand() / (double)RAND_MAX ;;
    p->l = build(level+1);
    p->r = build(level+1);
    return p;
  } else {
    return NULL;
  }
}

int traverse(struct node* p) {
  int count = 0;
  if (p == NULL) {
    return count;
  }
  if (p->l != NULL) {
    count += traverse(p->l);
  }
  if (p->r != NULL) {
    count += traverse(p->r);
  }
  if (p->val <= 0.5) {
    count++;
  }

  return count;
}

int main( ) {
  double time = -omp_get_wtime();
  struct node* h = build(0);
  time += omp_get_wtime();
  printf("time for building the tree: %f\n", time);

  time = -omp_get_wtime();
  int count = traverse(h);
  time += omp_get_wtime();

  printf("time for traversing the tree: %f\n", time);
  printf("final count: %d\n", count);


}
