#include <stdio.h>
#include <stdlib.h>

// O(1) Space Complexity
void constantSpace()
{
    int a = 10, b = 20, c;
    c = a + b;

    printf("\n[O(1)] Constant Space Used");
    printf("\nMemory used by variables: %lu bytes\n",
           sizeof(a) + sizeof(b) + sizeof(c));
}

// O(n) Space Complexity
void linearSpace(int n)
{
    int *arr = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }

    printf("\n[O(n)] Linear Space Used");
    printf("\nArray size: %d elements", n);
    printf("\nMemory used: %lu bytes\n", n * sizeof(int));

    free(arr);
}

// O(n^2) Space Complexity
void quadraticSpace(int n)
{
    int **matrix = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
    }

    printf("\n[O(n^2)] Quadratic Space Used");
    printf("\nMatrix size: %d x %d", n, n);
    printf("\nMemory used: %lu bytes\n", (unsigned long)(n * n * sizeof(int)));

    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int main()
{
    int n;

    printf("Enter input size (n): ");
    scanf("%d", &n);

    constantSpace();
    linearSpace(n);
    quadraticSpace(n);

    return 0;
}