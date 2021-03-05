#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  double token[] = {world_rank, -1};
  double received_token[2];
  if (world_rank%2 == 0) {
    int receive_from = world_rank - 1;
    if (receive_from<0) {
        receive_from = world_size+receive_from;
    }
    MPI_Recv(&received_token, 2, MPI_DOUBLE, receive_from, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %f, t %f from process %d\n", world_rank, received_token[0],
           received_token[1], receive_from);
  } else {
      int send_to = (world_rank + 1) % world_size;
    MPI_Send(&token, 2, MPI_DOUBLE, send_to, 0,
           MPI_COMM_WORLD);
  }

  if (world_rank%2 != 0) {
    int receive_from = world_rank - 1;
    if (receive_from<0) {
        receive_from = world_size+receive_from;
    }
    MPI_Recv(&received_token, 2, MPI_DOUBLE, receive_from, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %f, t %f from process %d\n", world_rank, received_token[0],
           received_token[1], receive_from);
  } else {
      int send_to = (world_rank + 1) % world_size;
    MPI_Send(&token, 2, MPI_DOUBLE, send_to, 0,
           MPI_COMM_WORLD);
  }

//   // Now process 0 can receive from the last process. This makes sure that at
//   // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
//   // deadlock)
//   if (world_rank == 0) {
//     MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
//              MPI_STATUS_IGNORE);
//     printf("Process %d received token %d from process %d\n", world_rank, token,
//            world_size - 1);
//   }
  MPI_Finalize();
}
