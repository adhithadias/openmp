#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void printArray(double* arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%f ", arr[i]);
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  MPI_Datatype block, blockType;
  MPI_Type_vector(2, 2, 2, MPI_DOUBLE, &block);
  MPI_Type_commit(&block);
  MPI_Type_create_resized(block, 0, 2 * sizeof(double), &blockType);
  MPI_Type_commit(&blockType);

  int size = 4;
  double number[size];
  if (world_rank == 0) {
    *number = -1;
    *(number + 1) = -2;
    *(number + 2) = -2;
    *(number + 3) = -2;
    MPI_Send(number, 1, blockType, 1, 0, MPI_COMM_WORLD);
  } else if (world_rank == 1) {
    MPI_Recv(number, 1, blockType, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %f from process 0\n", *number);
    printArray(number, size);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}