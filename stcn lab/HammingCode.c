#include <stdio.h>
#include <math.h>

int main()
{
    int data[20], datawithparity[20], recv[20];
    int m, r = 0, i, j, k, parity[10], error = 0;

    printf("Enter number of data bits: ");
    scanf("%d", &m);
    printf("Enter data bits (LSB first): ");
    for (i = 1; i <= m; i++)
        scanf("%d", &data[i]);

    // Find number of parity bits
    while ((int)pow(2, r) < (m + r + 1))
        r++;

    printf("\nNumber of parity bits: %d\n", r);

    // Insert parity bits into their positions
    j = k = 0;
    for (i = 1; i <= m + r; i++)
    {
        if (pow(2, j) == i)
        {
            datawithparity[i] = 0; // placeholder for parity bit
            j++;
        }
        else
        {
            datawithparity[i] = data[++k];
        }
    }

    // Calculate parity bits
    for (i = 0; i < r; i++)
    {
        int pos = pow(2, i);
        int count = 0;
        for (j = pos; j <= m + r; j++)
        {
            if (((j >> i) & 1) == 1)
                count += datawithparity[j];
        }
        datawithparity[pos] = count % 2; // even parity
    }

    printf("\nTransmitted Codeword: ");
    for (i = m + r; i >= 1; i--)
        printf("%d", datawithparity[i]);
    printf("\n");

    // Receiver side
    printf("\nEnter received bits one by one (LSB first): ");
    for (i = 1; i <= m + r; i++)
        scanf("%d", &recv[i]);

    // Check parity at receiver
    for (i = 0; i < r; i++)
    {
        int pos = pow(2, i);
        int count = 0;
        for (j = pos; j <= m + r; j++)
        {
            if (((j >> i) & 1) == 1)
                count += recv[j];
        }
        parity[i] = count % 2;
    }

    // Find error position
    for (i = 0; i < r; i++)
        error += parity[i] * pow(2, i);

    if (error == 0)
        printf("\nNo Error Detected ✅\n");
    else
    {
        printf("\nError detected at position: %d\n", error);
        recv[error] = (recv[error] == 0) ? 1 : 0;
        printf("Corrected Codeword: ");
        for (i = m + r; i >= 1; i--)
            printf("%d", recv[i]);
        printf("\n");
    }

    return 0;
}
