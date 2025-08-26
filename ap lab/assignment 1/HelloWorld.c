#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100

// Structure to hold student details
struct Student {
    char roll[20];
    int marks[4];
    int total;
    float percentage;
    float percentile;
    char grade[3];
};


int compare(const void *a, const void *b) {
    return ((struct Student*)b)->total - ((struct Student*)a)->total;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_csv>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    struct Student s[MAX];
    int n = 0;
    char line[256];

    // Read CSV data
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%d,%d,%d,%d",
               s[n].roll,
               &s[n].marks[0], &s[n].marks[1],
               &s[n].marks[2], &s[n].marks[3]) == 5) {
            s[n].total = s[n].marks[0] + s[n].marks[1] + s[n].marks[2] + s[n].marks[3];
            s[n].percentage = (s[n].total / 400.0f) * 100.0f;
            n++;
        }
    }
    fclose(fp);

    // Calculate mean
    float mean = 0;
    for (int i = 0; i < n; i++) mean += s[i].total;
    mean /= n;

    // Calculate standard deviation
    float sd = 0;
    for (int i = 0; i < n; i++) sd += pow(s[i].total - mean, 2);
    sd = sqrt(sd / n);

    // Sort students by total (descending) for percentile
    qsort(s, n, sizeof(struct Student), compare);

    // Assign percentile and grade
    for (int i = 0; i < n; i++) {
        s[i].percentile = ((float)(n - i) / n) * 100.0f;

        if (s[i].total >= mean + 3*sd) strcpy(s[i].grade, "A+");
        else if (s[i].total >= mean + 2*sd) strcpy(s[i].grade, "A");
        else if (s[i].total >= mean + sd) strcpy(s[i].grade, "B+");
        else if (s[i].total >= mean - sd) strcpy(s[i].grade, "B");
        else if (s[i].total >= mean - 2*sd) strcpy(s[i].grade, "C");
        else if (s[i].total >= mean - 3*sd) strcpy(s[i].grade, "P");
        else strcpy(s[i].grade, "F");
    }

    // Write back results into the same CSV
    fp = fopen(argv[1], "w");
    fprintf(fp, "Roll,T1,T2,T3,T4,Total,Percentage,Percentile,Grade\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%d,%d,%d,%d,%d,%.2f,%.2f,%s\n",
                s[i].roll,
                s[i].marks[0], s[i].marks[1],
                s[i].marks[2], s[i].marks[3],
                s[i].total, s[i].percentage, s[i].percentile, s[i].grade);
    }
    fclose(fp);

    // Compute statistics
    int gradeCount[7] = {0}; // A+,A,B+,B,C,P,F
    int highest = s[0].total, lowest = s[n-1].total;

    for (int i = 0; i < n; i++) {
        if (!strcmp(s[i].grade,"A+")) gradeCount[0]++;
        else if (!strcmp(s[i].grade,"A")) gradeCount[1]++;
        else if (!strcmp(s[i].grade,"B+")) gradeCount[2]++;
        else if (!strcmp(s[i].grade,"B")) gradeCount[3]++;
        else if (!strcmp(s[i].grade,"C")) gradeCount[4]++;
        else if (!strcmp(s[i].grade,"P")) gradeCount[5]++;
        else gradeCount[6]++;
    }

    // Display results
    printf("\nStatistics:\n");
    printf("Highest: %d, Lowest: %d, Average: %.2f\n", highest, lowest, mean);
    printf("Grade distribution:\n");
    printf("A+: %d, A: %d, B+: %d, B: %d, C: %d, P: %d, F: %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2],
           gradeCount[3], gradeCount[4], gradeCount[5], gradeCount[6]);

    return 0;
}
