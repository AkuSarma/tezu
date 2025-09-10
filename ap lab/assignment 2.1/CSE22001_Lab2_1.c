#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_RECORDS 1000
#define MAX_FIELD 100

// Transaction structure
typedef struct {
    char InvoiceNo[MAX_FIELD];
    char StockCode[MAX_FIELD];
    char Description[MAX_FIELD];
    int Quantity;
    char InvoiceDate[MAX_FIELD];
    double UnitPrice;
    int CustomerID;
    char Country[MAX_FIELD];
} Transaction;

Transaction records[MAX_RECORDS];
int recordCount = 0;

// Counters for sorting
long long comparisons = 0, swaps = 0;

// Helper function to parse a CSV line with quoted fields
int parseCSVLine(char *line, char fields[][MAX_FIELD], int maxFields) {
    int i = 0, j = 0, k = 0;
    int inQuotes = 0;
    char c;
    while ((c = line[j]) != '\0' && i < maxFields) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields[i][k] = '\0';
            i++; k = 0;
        } else {
            fields[i][k++] = c;
        }
        j++;
    }
    fields[i][k] = '\0';
    return i + 1;
}

// Function to read CSV file (robust parsing)
void readCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[MAX_LINE];
    fgets(line, MAX_LINE, fp); // skip header

    while (fgets(line, MAX_LINE, fp)) {
        if (recordCount >= MAX_RECORDS) break;
        char fields[8][MAX_FIELD] = {0};
        int n = parseCSVLine(line, fields, 8);
        if (n < 8) continue; // skip malformed lines

        strcpy(records[recordCount].InvoiceNo, fields[0]);
        strcpy(records[recordCount].StockCode, fields[1]);
        strcpy(records[recordCount].Description, fields[2]);
        records[recordCount].Quantity = atoi(fields[3]);
        strcpy(records[recordCount].InvoiceDate, fields[4]);
        records[recordCount].UnitPrice = atof(fields[5]);
        records[recordCount].CustomerID = atoi(fields[6]);
        strcpy(records[recordCount].Country, fields[7]);
        recordCount++;
    }

    fclose(fp);
}

// Display function
void displayRecords() {
    printf("%-10s %-10s %-20s %-8s %-20s %-10s %-10s %-10s\n",
           "InvoiceNo", "StockCode", "Description", "Quantity", "InvoiceDate", "UnitPrice", "CustomerID", "Country");
    for (int i = 0; i < recordCount; i++) {
        printf("%-10s %-10s %-20s %-8d %-20s %-10.2f %-10d %-10s\n",
               records[i].InvoiceNo, records[i].StockCode, records[i].Description,
               records[i].Quantity, records[i].InvoiceDate,
               records[i].UnitPrice, records[i].CustomerID, records[i].Country);
    }
}

// Comparator function based on column
int compare(Transaction a, Transaction b, int column) {
    comparisons++;
    switch (column) {
        case 1: return strcmp(a.InvoiceNo, b.InvoiceNo);
        case 2: return strcmp(a.StockCode, b.StockCode);
        case 3: return strcmp(a.Description, b.Description);
        case 4: return a.Quantity - b.Quantity;
        case 5: return strcmp(a.InvoiceDate, b.InvoiceDate);
        case 6: return (a.UnitPrice > b.UnitPrice) - (a.UnitPrice < b.UnitPrice);
        case 7: return a.CustomerID - b.CustomerID;
        case 8: return strcmp(a.Country, b.Country);
        default: return 0;
    }
}

// Merge Sort
void merge(Transaction arr[], int l, int m, int r, int column) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Transaction L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (compare(L[i], R[j], column) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
            swaps++;
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(Transaction arr[], int l, int r, int column) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m, column);
        mergeSort(arr, m + 1, r, column);
        merge(arr, l, m, r, column);
    }
}

// Quick Sort partition (fix pivot handling)
int partition(Transaction arr[], int low, int high, int column, int pivotType) {
    int pivotIdx;
    if (pivotType == 1) pivotIdx = low;
    else if (pivotType == 2) pivotIdx = high;
    else pivotIdx = low + rand() % (high - low + 1);

    // Move pivot to end
    if (pivotIdx != high) {
        Transaction tmp = arr[pivotIdx];
        arr[pivotIdx] = arr[high];
        arr[high] = tmp;
        swaps++;
    }
    Transaction pivot = arr[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compare(arr[j], pivot, column) <= 0) {
            i++;
            if (i != j) {
                Transaction temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                swaps++;
            }
        }
    }
    if (i + 1 != high) {
        Transaction temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        swaps++;
    }
    return (i + 1);
}

void quickSort(Transaction arr[], int low, int high, int column, int pivotType) {
    if (low < high) {
        int pi = partition(arr, low, high, column, pivotType);
        quickSort(arr, low, pi - 1, column, pivotType);
        quickSort(arr, pi + 1, high, column, pivotType);
    }
}

// Helper to trim whitespace and newline from a string (in-place)
void trim(char *str) {
    // Trim leading
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);

    // Trim trailing
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

// Binary Search on InvoiceNo
int binarySearch(char *key) {
    char trimmedKey[MAX_FIELD];
    strncpy(trimmedKey, key, MAX_FIELD-1);
    trimmedKey[MAX_FIELD-1] = '\0';
    trim(trimmedKey);

    int low = 0, high = recordCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        char midInvoice[MAX_FIELD];
        strncpy(midInvoice, records[mid].InvoiceNo, MAX_FIELD-1);
        midInvoice[MAX_FIELD-1] = '\0';
        trim(midInvoice);

        int cmp = strcmp(midInvoice, trimmedKey);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// Write to output file
void writeCSV(const char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "InvoiceNo,StockCode,Description,Quantity,InvoiceDate,UnitPrice,CustomerID,Country\n");
    for (int i = 0; i < recordCount; i++) {
        fprintf(fp, "%s,%s,%s,%d,%s,%.2f,%d,%s\n",
                records[i].InvoiceNo, records[i].StockCode, records[i].Description,
                records[i].Quantity, records[i].InvoiceDate,
                records[i].UnitPrice, records[i].CustomerID, records[i].Country);
    }
    fclose(fp);
}

int main() {
    srand(time(NULL));
    readCSV("A2.1_ecommerce - A2.1_ecommerce.csv");

    int choice, column, pivotType;
    char searchKey[MAX_FIELD];
    int sortedByInvoiceNo = 0;
    while (1) {
        printf("\nMenu:\n1. Display Records\n2. Merge Sort\n3. Quick Sort\n4. Binary Search (InvoiceNo)\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            displayRecords();
        } else if (choice == 2) {
            printf("Enter column (1.InvoiceNo 2.StockCode 3.Description 4.Quantity 5.InvoiceDate 6.UnitPrice 7.CustomerID 8.Country): ");
            scanf("%d", &column);
            comparisons = swaps = 0;
            mergeSort(records, 0, recordCount - 1, column);
            printf("Merge Sort Done. Comparisons: %lld, Swaps: %lld\n", comparisons, swaps);
            writeCSV("merge_sorted.csv");
            sortedByInvoiceNo = (column == 1);
        } else if (choice == 3) {
            printf("Enter column (1.InvoiceNo 2.StockCode 3.Description 4.Quantity 5.InvoiceDate 6.UnitPrice 7.CustomerID 8.Country): ");
            scanf("%d", &column);
            printf("Pivot type: 1.First element 2.Last element 3.Randomized: ");
            scanf("%d", &pivotType);
            comparisons = swaps = 0;
            quickSort(records, 0, recordCount - 1, column, pivotType);
            printf("Quick Sort Done. Comparisons: %lld, Swaps: %lld\n", comparisons, swaps);
            writeCSV("quick_sorted.csv");
            sortedByInvoiceNo = (column == 1);
        } else if (choice == 4) {
            if (!sortedByInvoiceNo) {
                printf("Warning: Records must be sorted by InvoiceNo for binary search to work correctly.\n");
            }
            printf("Enter InvoiceNo to search: ");
            scanf("%s", searchKey);
            trim(searchKey);
            int idx = binarySearch(searchKey);
            if (idx != -1) {
                printf("Found: %s %s %s %d %s %.2f %d %s\n",
                       records[idx].InvoiceNo, records[idx].StockCode, records[idx].Description,
                       records[idx].Quantity, records[idx].InvoiceDate,
                       records[idx].UnitPrice, records[idx].CustomerID, records[idx].Country);
            } else {
                printf("InvoiceNo not found.\n");
            }
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
