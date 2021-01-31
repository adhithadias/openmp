#include <stdio.h>
#include <algorithm>

void initialze_array(const int n, float *a, const float value);
void print_array(const int n, const float *a);

void initialze_array(const int n, float *a, const float value) {
    for (int i = 0; i < n; i++ ) {
      a[ i ] = value; 
   }
}

void print_array(const int n, const float *a) {
    printf("[");
    for (int i=0; i<std::min(10, n); i++){
        printf("%f ", a[i]);
    }
    printf("]\n");
}

