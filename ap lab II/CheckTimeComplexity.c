#include <stdio.h>
#include <time.h>


// O(1) - Constant Time
void constantTime(int n) {
    int x = n * n;   
}

// O(n) - Linear Time
void linearTime(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i;
    }
}

// O(n^2) - Quadratic Time
void quadraticTime(int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            count++;
        }
    }
}

int main() {
    clock_t start, end;
    double time_taken;

    printf("Input Size\tO(1)\t\tO(n)\t\tO(n^2)\n");
    printf("--------------------------------------------------------\n");

    for (int n = 1; n <= 20001; n += 5000) {

        // Constant Time
        start = clock();
        constantTime(n);
        end = clock();
        double t1 = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

        // Linear Time
        start = clock();
        linearTime(n);
        end = clock();
        double t2 = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

        // Quadratic Time
        start = clock();
        quadraticTime(n);
        end = clock();
        double t3 = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

        printf("%d\t\t%.6f\t%.6f\t%.6f\n", n, t1, t2, t3);
    }

    return 0;
}
