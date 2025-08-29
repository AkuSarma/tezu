#include <stdio.h>
#include <string.h>

void xor(char *dividend, char *divisor, int n) {
    for (int i = 1; i < n; i++)
        dividend[i] = (dividend[i] == divisor[i]) ? '0' : '1';
}

void crc(char *data, char *divisor, char *remainder) {
    int data_len = strlen(data);
    int divisor_len = strlen(divisor);

    char temp[100];
    strncpy(temp, data, divisor_len);
    temp[divisor_len] = '\0';

    for (int i = divisor_len; i <= data_len; i++) {
        if (temp[0] == '1')
            xor(temp, divisor, divisor_len);
        else
            xor(temp, "0000000000000000", divisor_len); // all zeros

        if (i != data_len)
            temp[divisor_len - 1] = data[i];
        memmove(temp, temp + 1, divisor_len - 1);
        temp[divisor_len - 1] = '\0';
    }
    strcpy(remainder, temp);
}

int main() {
    char data[100], divisor[20], transmitted[120], remainder[20];

    printf("Enter data bits: ");
    scanf("%s", data);
    printf("Enter divisor (generator polynomial): ");
    scanf("%s", divisor);

    int data_len = strlen(data);
    int divisor_len = strlen(divisor);

    // Append zeros to data
    strcpy(transmitted, data);
    for (int i = 0; i < divisor_len - 1; i++)
        transmitted[data_len + i] = '0';
    transmitted[data_len + divisor_len - 1] = '\0';

    crc(transmitted, divisor, remainder);

    printf("CRC remainder: %s\n", remainder);

    // Append remainder to data to form codeword
    strcpy(transmitted, data);
    strcat(transmitted, remainder);
    printf("Transmitted codeword: %s\n", transmitted);

    // Receiver side: check for errors
    char received[120], check_rem[20];
    printf("Enter received codeword: ");
    scanf("%s", received);

    crc(received, divisor, check_rem);

    int error = 0;
    for (int i = 0; i < divisor_len - 1; i++) {
        if (check_rem[i] != '0') {
            error = 1;
            break;
        }
    }
    if (error)
        printf("Error detected in received codeword.\n");
    else
        printf("No error detected in received codeword.\n");

    return 0;
}