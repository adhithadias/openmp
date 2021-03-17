#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void printArray(double *a, int size) {
  int i;
  for (i = 0; i < size; i++) {
    printf("%.2f ", *(a + i));
  }
  printf("\n\n\n");
}

double *makeArray(int size, int value, int p) {
  double *arr = (double *)malloc(size * sizeof(double));

  int r, c;
  for (r = 0; r < size; r++) {
    if (value == -1) {
      *(arr + r) = (double)(p);
    } else {
      *(arr + r) = (double)(value);
    }
  }

  return arr;
}

double sum_reduction(double *a, int size) {
  double sum = 0;
  int i;
  for (i = 0; i < size; i++) {
    sum += a[i];
  }
  return sum;
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_size;
  int p;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &p);
  MPI_Get_processor_name(processor_name, &name_len);

  int array_size = SIZE / world_size;
  double *a = makeArray(array_size, -1, p);
  double p_sum = 0;
  double t1, t2;

  MPI_Barrier(MPI_COMM_WORLD);
  t1 = MPI_Wtime();

  // sum reduction inside the process
  p_sum = sum_reduction(a, array_size);

  // reduce all of the local sums into the global sum
  double final_sum;
  MPI_Reduce(&p_sum, &final_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);


  MPI_Barrier(MPI_COMM_WORLD);
  t2 = MPI_Wtime();

  // print the sum on each process
  printf("Local sum for process/world %d/%d: %f\n", p, world_size, p_sum);

  // print the result
  if (p == 0) {
    printf("1st execution -- total sum = %f, time taken: %.10f\n", final_sum, t2-t1);
  }

  /********************************************************************
   * 2nd time execution
   *********************************************************************/
  MPI_Barrier(MPI_COMM_WORLD);
  t1 = MPI_Wtime();
  p_sum = sum_reduction(a, array_size);
  MPI_Reduce(&p_sum, &final_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  t2 = MPI_Wtime();

  // print the result
  if (p == 0) {
    printf("2nd execution -- total sum = %f, time taken: %.10f\n", final_sum, t2-t1);
  }

  // Clean up
  free(a);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
