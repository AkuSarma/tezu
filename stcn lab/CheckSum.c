#include <stdio.h>

int onesComplement(int sum)
{
    return (~sum) & 0xFF; // For 8-bit words
}

void checksum()
{
    int n, data[10], sum = 0;
    printf("\nEnter number of 8-bit data blocks: ");
    scanf("%d", &n);

    printf("Enter data blocks (in hexadecimal, e.g., FF, 5A):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%x", &data[i]);
        sum += data[i];
        if (sum > 0xFF)
        { // Wrap around carry
            sum = (sum & 0xFF) + 1;
        }
    }

    int check = onesComplement(sum);
    printf("Checksum = %02X\n", check);

    // Receiver side verification
    sum = 0;
    printf("Enter received data blocks (including checksum):\n");
    for (int i = 0; i < n + 1; i++)
    {
        scanf("%x", &data[i]);
        sum += data[i];
        if (sum > 0xFF)
            sum = (sum & 0xFF) + 1;
    }

    if (onesComplement(sum) == 0)
        printf("No Error Detected\n");
    else
        printf("Error Detected\n");
}
