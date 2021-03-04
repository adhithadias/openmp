#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 10000
#define NUM_THREADS 20

typedef struct Q {
   int* q;
   int pos;
   int size;
} Q;

struct Q* initQ(int n) {
   int i;
   struct Q *newQ = (struct Q *) malloc(sizeof(Q));   
   newQ->q = (int*) malloc(sizeof(int)*n);
   newQ->pos = -1;
   newQ->size = n-1;
   return newQ;
}

void putWork(struct Q* workQ) {
   if (workQ->pos < (workQ->size)) {
      workQ->pos++;
      workQ->q[workQ->pos] = (int) (rand( )%2*(workQ->pos/1000));
   } else {
      printf("ERROR: attempt to add Q element%d\n", workQ->pos+1);
   }
}

int getWork(struct Q* workQ) {
   int w = 0;
   #pragma omp critical
   {
      if (workQ->pos > -1) {
         w = workQ->q[workQ->pos];
         workQ->pos--;
      } else {
         printf("ERROR: attempt to get work from empty Q%d\n", workQ->pos);
      }
   }
   return w;
}

int main(int argc, char *argv[]) {
   struct Q* queue = initQ(QUEUE_SIZE);

   for (int i=0; i<QUEUE_SIZE; i++) {
      putWork(queue);
   }


   double time = -omp_get_wtime();
   while (queue->pos >= 0) {
      int work = getWork(queue);
      usleep(work*100);
   }
   time += omp_get_wtime();
   printf("time for serial execution: %f\n", time);

   // put the position to the end of the queue
   // to calculate time for parallel execution
   // this way we know that queue values are not changed
   queue->pos = QUEUE_SIZE-1;

   omp_set_num_threads(NUM_THREADS);

   time = -omp_get_wtime();
   #pragma omp parallel
   {
      while (queue->pos >= 0) {
         int work = getWork(queue);
         // printf("work: %d\n", work);
         usleep(work*100);
      }
   }
   time += omp_get_wtime();
   printf("time for parallel execution: %f\n", time);

}
