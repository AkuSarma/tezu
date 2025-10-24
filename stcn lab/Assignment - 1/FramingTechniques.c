#include <stdio.h>
#include <string.h>

// ---------- BIT STUFFING ----------
void bitStuffing() {
    char data[100], stuffed[200];
    int i, j = 0, count = 0;
    printf("\nEnter binary data: ");
    scanf("%s", data);

    stuffed[j++] = 'F'; // starting flag
    for (i = 0; i < strlen(data); i++) {
        stuffed[j++] = data[i];
        if (data[i] == '1') {
            count++;
            if (count == 5) {
                stuffed[j++] = '0';
                count = 0;
            }
        } else {
            count = 0;
        }
    }
    stuffed[j++] = 'F'; // ending flag
    stuffed[j] = '\0';
    printf("Bit Stuffed Data: %s\n", stuffed);
}

// ---------- BYTE STUFFING ----------
void byteStuffing() {
    char data[100], stuffed[200];
    int i, j = 0;
    printf("\nEnter data string: ");
    scanf("%s", data);

    stuffed[j++] = 'F'; // starting flag
    for (i = 0; i < strlen(data); i++) {
        if (data[i] == 'F' || data[i] == 'E') {
            stuffed[j++] = 'E'; // escape character
        }
        stuffed[j++] = data[i];
    }
    stuffed[j++] = 'F'; // ending flag
    stuffed[j] = '\0';
    printf("Byte Stuffed Data: %s\n", stuffed);
}

// ---------- MAIN ----------
int main() {
    int choice;
    printf("Framing Techniques\n");
    printf("1. Bit Stuffing\n");
    printf("2. Byte Stuffing\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: bitStuffing(); break;
        case 2: byteStuffing(); break;
        default: printf("Invalid choice!\n");
    }
    return 0;
}
