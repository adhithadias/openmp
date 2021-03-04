#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 20

void doWork(int t) {
   usleep(t*10000);
}

int* initWork(int n) {
   int i;
   double r;
   int* wA = (int *) malloc(sizeof(int)*n);   
   for (i = 0; i < n; i++) {
      wA[i] = (int) rand( )%2*i/(n/10);
   }
   return wA;
}

int main (int argc, char *argv[]) {
   omp_set_num_threads(NUM_THREADS);

   int i;
   int *w = initWork(1000);

   double time = 0;
   
   // serial execution
   time = -omp_get_wtime();
   for (i = 0; i < 1000; i++) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("SERIAL EXECUTION TIME: %f\n", time);

   // parallel execution
   time = -omp_get_wtime();
   #pragma omp parallel sections
   {
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
      #pragma omp section
      {
         int i;
         for (i = omp_get_thread_num(); i < 1000; i+=NUM_THREADS) {
            doWork(w[i]);
         }
      }
   }
   time += omp_get_wtime();
   printf("PARALLEL SECTIONS EXECUTION TIME: %f\n", time);

}

