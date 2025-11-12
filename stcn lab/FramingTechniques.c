#include <stdio.h>
#include <string.h>

// ---------------- BIT STUFFING ----------------
void bitStuffing(char *input) {
    char output[100];
    int count = 0, j = 0;
    output[j++] = '0'; // Start flag bit
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '0'; // End flag bit

    for (int i = 0; i < strlen(input); i++) {
        output[j++] = input[i];
        if (input[i] == '1') {
            count++;
            if (count == 5) {
                output[j++] = '0'; // Stuff a 0 after five consecutive 1s
                count = 0;
            }
        } else {
            count = 0;
        }
    }

    // Add end flag
    output[j++] = '0';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '1';
    output[j++] = '0';
    output[j] = '\0';

    printf("\nBit Stuffed Data: %s\n", output);
}

// ---------------- BYTE STUFFING ----------------
void byteStuffing(char *input) {
    char flag = 'F', esc = 'E';
    char output[100];
    int j = 0;

    output[j++] = flag; // Start flag
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == flag || input[i] == esc) {
            output[j++] = esc; // Stuff escape character
        }
        output[j++] = input[i];
    }
    output[j++] = flag; // End flag
    output[j] = '\0';

    printf("\nByte Stuffed Data: %s\n", output);
}

// ---------------- MAIN ----------------
int main() {
    char bitInput[50], byteInput[50];

    printf("Enter bit stream (e.g., 01111110110): ");
    scanf("%s", bitInput);
    bitStuffing(bitInput);

    printf("\nEnter byte stream (string): ");
    scanf("%s", byteInput);
    byteStuffing(byteInput);

    return 0;
}
