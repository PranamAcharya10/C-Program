#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define n 90

void generate_list(int a[], int x)
{
    int i, j, t;
    for (i = 0; i < x; i++)
    {
        a[i] = i;
    }
    for (int i = 0; i < x; i++)
    {
        j = rand() % x;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];

    int i = low - 1;
    int j;

    for (j = low; j <= high - 1; j++)
    {

        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}

void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

#pragma omp task firstprivate(arr, low, pi)
        quicksort(arr, low, pi - 1);

#pragma omp task firstprivate(arr, pi, high)
        quicksort(arr, pi + 1, high);

#pragma omp taskwait
    }
}

int main(int argc, char const *argv[])
{
    int arr[n], B[n];

    generate_list(arr, n);

    for (int i = 0; i < n; i++)
    {
        B[i] = arr[i];
    }
    double start = omp_get_wtime();

#pragma omp parallel
    {
#pragma omp single
        quicksort(B, 0, n - 1);
    }

    double end = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        printf("%d\t", B[i]);
    }
    printf("\nExecution time: %f seconds\n", end - start);

    return 0;
}
