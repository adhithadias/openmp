#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_THREADS 16

void printArray(double* a, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%.2f ", *(a + i * cols + j));
    }
    printf("\n");
  }
  printf("\n\n\n");
}

double* makeArray(int rows, int cols, int zero) {
  double* arr = (double*)malloc(rows * cols * sizeof(double));

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int initialVal = zero ? r * cols + c : 0;
      // printf("%d\n", initialVal);
      *(arr + r * cols + c) = (double)initialVal;
    }
  }

  return arr;
}

typedef struct Coord {
  int row;
  int col;
} CoordT;

int main(int argc, char* argv[]) {

  const int ARRAY_ROWS = 8;
  const int ARRAY_COLS = 8;
  const int IS_ZERO = 0;
  const int NOT_ZERO = 1;

  double* a = makeArray(ARRAY_ROWS, ARRAY_COLS, NOT_ZERO);
  double* b = makeArray(ARRAY_ROWS, ARRAY_COLS, NOT_ZERO);
  double* c = makeArray(ARRAY_ROWS, ARRAY_COLS, IS_ZERO);

  // printArray(a, ARRAY_ROWS, ARRAY_COLS);
  // printArray(b, ARRAY_ROWS, ARRAY_COLS);

  omp_set_num_threads(NUM_THREADS);
  // matrix multiply goes here
  // initial checker board pattern creation
  int sqrt_num_threads = (int) sqrt(NUM_THREADS);
  const int CORE_ROWS = sqrt_num_threads;
  const int CORE_COLS = sqrt_num_threads;
  const int LOCAL_ROWS = ARRAY_ROWS / CORE_ROWS;
  const int LOCAL_COLS = ARRAY_COLS / CORE_COLS;
  const int ROOT_P = CORE_ROWS;  // could be CORE_ROWS, sqrt(16)
  printf(
      "core rows: %d, core cols: %d, array rows: %d, array cols: %d, "
      "local_rows: %d, local_cols: %d, root_p: %d\n",
      CORE_ROWS, CORE_COLS, ARRAY_ROWS, ARRAY_COLS, LOCAL_ROWS, LOCAL_COLS,
      ROOT_P);

  // here, we will not actually move data, instead we will change the data that
  // each thread initially access to make it access the data that it would were
  // it an MPI process and the data was actually shifted
  double timeTaken = -omp_get_wtime( );
  #pragma omp parallel for shared(a, b, c)
  for (int threadn = 0; threadn < NUM_THREADS; threadn++) {

    int thread_row = threadn / sqrt_num_threads;
    int thread_col = threadn % sqrt_num_threads;

    struct Coord myThreadCoord = {thread_row, thread_col};

    // printf("My thread row: %d, col: %d\n", myThreadCoord.row,
    //        myThreadCoord.col);

    // rows of a matrix should be shifted left by block_row number of blocks
    struct Coord blockPointerA;
    blockPointerA.row = myThreadCoord.row;
    blockPointerA.col =
        (myThreadCoord.col + myThreadCoord.row) % sqrt_num_threads;
    // printf("A pointer block row: %d, col: %d\n", blockPointerA.row,
    //        blockPointerA.col);

    // columns of b matrix should be shifted up by block_col number of columns
    struct Coord blockPointerB;
    blockPointerB.row =
        (myThreadCoord.row + myThreadCoord.col) % sqrt_num_threads;
    blockPointerB.col = myThreadCoord.col;
    // printf("B pointer block row: %d, col: %d\n", blockPointerB.row,
    //        blockPointerB.col);

    // do the first multiply
    // matrix_multiply(c, a, b, blockPointerA, blockPointerB);
    int i, j, k, l;

    int crBegin = myThreadCoord.row * LOCAL_ROWS * ARRAY_COLS;
    int ccBegin = myThreadCoord.col * LOCAL_COLS;

    // running the loop for sqrt(T) steps
    for (l = 0; l < sqrt_num_threads; l++) {
      int arBegin = blockPointerA.row * LOCAL_ROWS * ARRAY_COLS;
      int acBegin = blockPointerA.col * LOCAL_COLS;

      int brBegin = blockPointerB.row * LOCAL_ROWS * ARRAY_COLS;
      int bcBegin = blockPointerB.col * LOCAL_COLS;

      // matrix multiplication for the tile corresponding to the thread
      for (i = 0; i < LOCAL_ROWS; i++) {
        for (j = 0; j < LOCAL_COLS; j++) {
          double comp = 0;
          for (k = 0; k < LOCAL_COLS; k++) {
            // printf("a: %f    b: %f\n",
            //        *(a + arBegin + acBegin + i * ARRAY_COLS + k),
            //        *(b + brBegin + bcBegin + k * ARRAY_COLS + j));
            comp += *(a + arBegin + acBegin + i * ARRAY_COLS + k) *
                    *(b + brBegin + bcBegin + k * ARRAY_COLS + j);
          }
          *(c + crBegin + ccBegin + i * ARRAY_COLS + j) += comp;
        }
      }

      // do the shift by one of A and B
      blockPointerA.col = (blockPointerA.col + 1) % sqrt_num_threads;
      blockPointerB.row = (blockPointerB.row + 1) % sqrt_num_threads;
    }
  }

  timeTaken = (timeTaken + omp_get_wtime( ));
   printf("time taken for matrix multiply: %.10f\n", timeTaken);

  printArray(c, ARRAY_ROWS, ARRAY_COLS);
  fflush(stdout);

  return EXIT_SUCCESS;
}
