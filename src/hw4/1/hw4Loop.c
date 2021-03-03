#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNK 50
#define NUM_THREADS 4

void doWork(int t) {
   usleep(t*100000);
}

int* initWork(int n) {
   int i;
   double r;
   int* wA = (int *) malloc(sizeof(int)*n);   
   for (i = 0; i < n; i++) {
      wA[i] = rand( )%2*i/(n/10);
   }
   return wA;
}

int main (int argc, char *argv[]) {

   omp_set_num_threads(NUM_THREADS);

   int i;
   int *w = initWork(1000);

   double time = 0;
   
   // SERIAL EXECUTION
   time = -omp_get_wtime();
   for (i = 0; i < 1000; i+=50) {
      printf("w[%d] = %d, thread_num: %d\n", i, w[i], omp_get_thread_num());
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("SERIAL EXECUTION TIME: %f\n", time);

   // STATIC SCHEDULING WITH DEFAULT CHUNK SIZE
   time = -omp_get_wtime();
   #pragma omp parallel for schedule(static)
   for (i = 0; i < 1000; i+=50) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("STATIC SCHEDULE EXECUTION TIME WITH DEFAULT CHUNK: %f\n", time);

   // STATIC SCHEDULING WITH CHUNK SIZE 50
   time = -omp_get_wtime();
   #pragma omp parallel for schedule(static, CHUNK)
   for (i = 0; i < 1000; i+=50) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("STATIC SCHEDULE EXECUTION TIME WITH CHUNK SIZE 50: %f\n", time);

   // DYNAMIC SCHEDULING WITH DEFAULT CHUNK SIZE
   time = -omp_get_wtime();
   #pragma omp parallel for schedule(dynamic)
   for (i = 0; i < 1000; i+=50) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("STATIC SCHEDULE EXECUTION TIME WITH DEFAULT CHUNK: %f\n", time);

   // DYNAMIC SCHEDULE WITH CHUNK SIZE 50
   time = -omp_get_wtime();
   #pragma omp parallel for schedule(dynamic, CHUNK)
   for (i = 0; i < 1000; i+=50) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("DYNAMIC SCHEDULE EXECUTION TIME WITH CHUNK SIZE 50: %f\n", time);

   // GUIDED SCHEDULING
   time = -omp_get_wtime();
   #pragma omp parallel for schedule(guided)
   for (i = 0; i < 1000; i+=50) {
      doWork(w[i]);
   }
   time += omp_get_wtime();
   printf("GUIDED SCHEDULE EXECUTION TIME: %f\n", time);

}

