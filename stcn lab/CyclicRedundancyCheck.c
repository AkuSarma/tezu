#include <stdio.h>
#include <string.h>

void xorDivision(char dividend[], char divisor[], char remainder[])
{
    int i, j;
    int len_div = strlen(divisor);

    for (i = 0; i < len_div; i++)
        remainder[i] = dividend[i];

    int len_msg = strlen(dividend);
    for (i = len_div; i <= len_msg; i++)
    {
        if (remainder[0] == '1')
        {
            for (j = 1; j < len_div; j++)
                remainder[j - 1] = (remainder[j] == divisor[j] ? '0' : '1');
        }
        else
        {
            for (j = 1; j < len_div; j++)
                remainder[j - 1] = remainder[j];
        }
        remainder[len_div - 1] = dividend[i];
    }
    remainder[len_div - 1] = '\0';
}

void crc()
{
    char data[50], divisor[50], dividend[50], remainder[50], codeword[50];
    int len_data, len_div, i;

    printf("\nEnter Data: ");
    scanf("%s", data);
    printf("Enter Divisor (Generator): ");
    scanf("%s", divisor);

    len_data = strlen(data);
    len_div = strlen(divisor);

    strcpy(dividend, data);
    for (i = 0; i < len_div - 1; i++)
        strcat(dividend, "0");

    xorDivision(dividend, divisor, remainder);

    for (i = 0; i < len_div - 1; i++)
        data[len_data + i] = remainder[i];
    data[len_data + len_div - 1] = '\0';

    printf("Transmitted Codeword: %s\n", data);
}
