#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

#define MAXLEVEL 18
#define NUM_THREADS 8

struct node {
  double val;
  struct node* l;
  struct node* r;
} nodeT;

void print(struct node* p, int level) {
  if (p == NULL) return;
  printf("%f, level: %d\n", p->val, level);

  if (p->l == NULL) return;
  else print(p->l, level+1);

  if (p->r == NULL) return;
  else print(p->r, level+1);

}

struct node* build(int level) {

  if (level < MAXLEVEL) {
    struct node* p = (struct node*) malloc(sizeof(nodeT));
    p->val = (double)rand() / (double)RAND_MAX ;
    p->l = build(level+1);
    p->r = build(level+1);
    return p;
  } else {
    return NULL;
  }
}

struct node* build_parallel(int level) {
  if (level >= MAXLEVEL) {
    return NULL;
  }

  bool par_exec = ( pow(2,level) <= omp_get_num_threads() );

  struct node* p = (struct node*) malloc(sizeof(nodeT));
  p->val = (double)rand() / (double)RAND_MAX ;

  if (par_exec){

    #pragma omp task shared(p)
    p->l = build_parallel(level+1);
    #pragma omp task shared(p)
    p->r = build_parallel(level+1);

    #pragma omp taskwait

  } else {
    p->l = build(level+1);
    p->r = build(level+1);

  }

  return p;

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

int traverse_parallel(struct node* p, int level) {
  int count =0;
  int lcount = 0;
  int rcount = 0;

  if (p == NULL) {
    return count;
  }

  int par_exec = ( pow(2,level) <= omp_get_num_threads() );

  if (par_exec){

    #pragma omp task shared(lcount)
    lcount = traverse_parallel(p->l, level+1);
    #pragma omp task shared(rcount)
    rcount = traverse_parallel(p->r, level+1);

    #pragma omp taskwait
    count = lcount + rcount;

    if (p->val < 0.5) count++;
  } else {
    count += traverse(p);
  }

  return count;

}

int main( ) {

  omp_set_num_threads(NUM_THREADS);

  struct node* h;
  // parallel building of the tree
  double time = -omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp single
    h = build_parallel(0);
  }
  time += omp_get_wtime();
  printf("time for building the tree: %f\n", time);

  // print(h, 0);

  int count = 0;

  // parallel traversing of the tree
  time = -omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp single
    count += traverse_parallel(h, 0);
  }

  time += omp_get_wtime();

  printf("time for traversing the tree: %f\n", time);
  printf("final count: %d\n", count);


}
