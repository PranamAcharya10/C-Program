//Parallel Prefix Sum 
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define NUM_ELEMENTS 10

int main() {
    int original_array[NUM_ELEMENTS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int seq_sum[NUM_ELEMENTS];
    int sum = 0;
    int i = 0;

    // Sequential Solution
    printf("Sequential Prefix Array: ");

    while (i < NUM_ELEMENTS) {
        sum += original_array[i];
        seq_sum[i] = sum;
        printf("%d ", seq_sum[i++]);
    }
    printf("\n");

    // Parallel Solution
    int par_sum[NUM_ELEMENTS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int limit = (int)log2(NUM_ELEMENTS) + 1;

    for (int depth = 1; depth <= limit; depth++) {
        // Wait for each thread from last depth to finish
        #pragma omp barrier
        int step = (int)pow(2, depth - 1);
        int temp_sum[NUM_ELEMENTS] = {0};

        // Calculate sums for current depth, stored in temp_sum
        #pragma omp parallel for shared(par_sum, temp_sum)
        for (int k = step; k < NUM_ELEMENTS; k++) {
            temp_sum[k] = par_sum[k - step] + par_sum[k];
        }

        // Update each element in par_sum with temp_sum values
        #pragma omp parallel for shared(par_sum, temp_sum)
        for (int k = step; k < NUM_ELEMENTS; k++) {
            par_sum[k] = temp_sum[k];
        }
    }

    // Print par_sum array
    printf("Parallel Prefix Array: ");
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        printf("%d ", par_sum[i]);
    }

    return 0;
}
