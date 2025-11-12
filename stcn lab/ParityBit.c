#include <stdio.h>
#include <string.h>

void parityBit()
{
    char data[20];
    int count = 0;

    printf("Enter data bits (e.g., 101001): ");
    scanf("%s", data);

    // Count number of 1s
    for (int i = 0; i < strlen(data); i++)
        if (data[i] == '1')
            count++;

    // Even parity (add 1 if count is odd)
    if (count % 2 == 0)
        printf("Even Parity Bit = 0\nFinal Data (Even Parity): %s0\n", data);
    else
        printf("Even Parity Bit = 1\nFinal Data (Even Parity): %s1\n", data);
}
