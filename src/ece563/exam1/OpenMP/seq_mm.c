#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void printArray(double* a, int rows, int cols) {
   for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
         printf("%.2f ", *(a + i*cols + j));
      }
      printf("\n");
   }
   printf("\n\n\n");
}

double* makeArray(int rows, int cols, int zero) {
   double* arr = (double*) malloc(rows*cols*sizeof(double));

   for (int r=0; r<rows; r++) {
      for (int c=0; c<cols; c++) {
         int initialVal = zero ? r*cols + c : 0;
         *(arr + r*cols + c) = (double) initialVal;
      }
   }

   return arr;
}

typedef struct Coord {
   int row;
   int col;
} CoordT;

int main(int argc, char *argv[]) {

   const int ARRAY_ROWS = 8;
   const int ARRAY_COLS = 8;

   const int IS_ZERO = 0;
   const int NOT_ZERO = 1;

   double* a = makeArray(ARRAY_ROWS, ARRAY_COLS, NOT_ZERO);
   double* b = makeArray(ARRAY_ROWS, ARRAY_COLS, NOT_ZERO);
   double* c = makeArray(ARRAY_ROWS, ARRAY_COLS, IS_ZERO);

   // #pragma omp parallel for collapse(2) 
   /* 
     adjust starting positions. This is similar to what the initial shifts to with MPI,
     but we are adjusing the data accessed by threads instead of shifting the data.
   */ 
   double timeTaken = -omp_get_wtime( );
   for (int i=0; i<ARRAY_ROWS; i++) {
      for (int j=0; j<ARRAY_COLS; j++) {
         double comp = 0.;
         for (int k=0; k<ARRAY_COLS; k++) {
            comp += *(a + i*ARRAY_COLS + k) * *(b + k*ARRAY_COLS + j);
         }
         *(c + i*ARRAY_COLS + j) += comp;
      }
   }
   timeTaken = (timeTaken + omp_get_wtime( ));
   printf("time taken for matrix multiply: %.10f\n", timeTaken);
   printArray(c, ARRAY_ROWS, ARRAY_COLS);
   fflush(stdout);
}
