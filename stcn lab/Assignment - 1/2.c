#include <stdio.h>
#include <string.h>

// ---------- PARITY BIT ----------
void parityBit()
{
    char data[50];
    int i, count = 0;
    printf("\nEnter binary data: ");
    scanf("%s", data);
    for (i = 0; i < strlen(data); i++)
        if (data[i] == '1')
            count++;
    printf("Even Parity Bit: %d\n", count % 2 == 0 ? 0 : 1);
    printf("Odd Parity Bit: %d\n", count % 2 == 0 ? 1 : 0);
}

// ---------- CHECKSUM ----------
void checksum()
{
    int data[10], n, i, sum = 0, checksum;
    printf("\nEnter number of data bytes: ");
