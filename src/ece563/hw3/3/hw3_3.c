#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define m 10
#define n 10
#define N 100

int pointwise_mul_and_sum(int array2d[m][n]);
void multiplyMatrices (int m1[][N], int m2[][1], int result[][1], int r1, int c1, int r2, int c3);

int main (int argc, char *argv) {

    int x[m][n];
    for (int i=0; i<m; i++) {
        for (int j=0; j<n; j++) {
            x[i][j] = 1;
        }
    }

    int y[1][N], z[N][1];
    int result[1][1];
    for (int i=0; i<N; i++) {
        y[0][i] = 1;
        z[i][0] = 1;
    }

    #pragma omp parallel sections
    {

        #pragma omp section         // executed by one thread, parallel to other sections
        pointwise_mul_and_sum(x);

        #pragma omp section         // executed by one thread, parallel to other sections
        multiplyMatrices(y, z, result, 1, N, N, 1);

    }

    return 0;
}

void multiplyMatrices (int m1[][N], int m2[][1], int result[][1], int r1, int c1, int r2, int c3) {
    printf("SEC 2 -- thread num: %d, thread count: %d\n", omp_get_thread_num(), omp_get_num_threads());
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c3; j++) {
            int sum = 0;
            for (int k=0; k<c1; k++) {
                sum += m1[i][k] + m2[k][j];
            }
            result[i][j] = sum;
        }
    }
    printf("SEC 2 -- result: %d\n", result[0][0]);

}

int pointwise_mul_and_sum(int array2d[m][n]) {
    printf("SEC 1 -- thread num: %d, thread count: %d\n", omp_get_thread_num(), omp_get_num_threads());
    int sum = 0;
    for (int i = 0; i<m; i++) {
        for (int j=0; j<n; j++) {
            sum += array2d[i][j]*array2d[i][j];
        }
    }
    printf("SEC 1 -- result: %d\n", sum);
    return sum;
}