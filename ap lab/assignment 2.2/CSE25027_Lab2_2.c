#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char dnabase; 

unsigned char encodeBase(char c)
{
    switch (c)
    {
    case 'A':
        return 0b00;
    case 'C':
        return 0b01;
    case 'G':
        return 0b10;
    case 'T':
        return 0b11;
    default:
        return 0;
    }
}

char decodeBase(unsigned char b)
{
    switch (b)
    {
    case 0b00:
        return 'A';
    case 0b01:
        return 'C';
    case 0b10:
        return 'G';
    case 0b11:
        return 'T';
    default:
        return '?';
    }
}

dnabase *encodeDNA(const char *s, int len, int *bytesUsed)
{
    int totalBits = len * 2;
    *bytesUsed = (totalBits + 7) / 8;
    dnabase *packed = (dnabase *)calloc(*bytesUsed, sizeof(dnabase));

    int bitPos = 0;
    for (int i = 0; i < len; i++)
    {
        unsigned char val = encodeBase(s[i]);
        int byteIndex = bitPos / 8;
        int offset = bitPos % 8;
        packed[byteIndex] |= (val << offset);
        bitPos += 2;
    }
    return packed;
}

char *decodeDNA(const dnabase *packed, int len)
{
    char *result = (char *)malloc((len + 1) * sizeof(char));
    int bitPos = 0;
    for (int i = 0; i < len; i++)
    {
        int byteIndex = bitPos / 8;
        int offset = bitPos % 8;
        unsigned char val = (packed[byteIndex] >> offset) & 0b11;
        result[i] = decodeBase(val);
        bitPos += 2;
    }
    result[len] = '\0';
    return result;
}

int LCSLength(const char *X, const char *Y, int **dp)
{
    int m = strlen(X), n = strlen(Y);
    *dp = (int *)calloc((m + 1) * (n + 1), sizeof(int));
    int *table = *dp;

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (X[i - 1] == Y[j - 1])
                table[i * (n + 1) + j] = table[(i - 1) * (n + 1) + (j - 1)] + 1;
            else
            {
                int up = table[(i - 1) * (n + 1) + j];
                int left = table[i * (n + 1) + (j - 1)];
                table[i * (n + 1) + j] = (up > left) ? up : left;
            }
        }
    }
    int result = table[m * (n + 1) + n];
    return result;
}

char *LCSubstring(const char *X, const char *Y)
{
    int m = strlen(X), n = strlen(Y);
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    
    for (int i = 0; i <= m; i++)
        dp[i] = (int *)calloc((n + 1), sizeof(int));

    int maxLen = 0, endPos = 0;
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (X[i - 1] == Y[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLen)
                {
                    maxLen = dp[i][j];
                    endPos = i;
                }
            }
        }
    }

    char *substr = (char *)malloc((maxLen + 1) * sizeof(char));
    strncpy(substr, X + endPos - maxLen, maxLen);
    substr[maxLen] = '\0';

    for (int i = 0; i <= m; i++)
        free(dp[i]);
    free(dp);

    return substr;
}

int main()
{
    FILE *fin = fopen("A2.2_dna.txt", "r");
    if (!fin)
    {
        printf("File not found!\n");
        return 1;
    }

    char sequences[10][1024];
    int count = 0;
    while (fgets(sequences[count], sizeof(sequences[count]), fin))
    {
        sequences[count][strcspn(sequences[count], "\r\n")] = 0;
        if (strlen(sequences[count]) > 0)
            count++;
    }
    fclose(fin);

    printf("Read %d sequences:\n", count);
    for (int i = 0; i < count; i++)
    {
        int bytesUsed;
        dnabase *encoded = encodeDNA(sequences[i], strlen(sequences[i]), &bytesUsed);
        printf("Sequence %d: %s | Stored in %d bytes\n", i + 1, sequences[i], bytesUsed);
        free(encoded);
    }

    int maxLCS = -1;
    int bestA = 0, bestB = 1;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            int *dp;
            int lcsLen = LCSLength(sequences[i], sequences[j], &dp);
            printf("LCS length between Seq%d and Seq%d = %d\n", i + 1, j + 1, lcsLen);
            free(dp);
            if (lcsLen > maxLCS)
            {
                maxLCS = lcsLen;
                bestA = i;
                bestB = j;
            }
        }
    }

    printf("\nBest pair: Seq%d & Seq%d with LCS length %d\n", bestA + 1, bestB + 1, maxLCS);

    char *substr = LCSubstring(sequences[bestA], sequences[bestB]);
    printf("Longest Common Substring between best pair: %s (length %lu)\n", substr, strlen(substr));
    free(substr);

    return 0;
}
