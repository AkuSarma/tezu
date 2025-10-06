#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TREE_HT 256
#define MAX_CHAR 256

// Huffman Tree Node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Min Heap structure
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Create new node
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->data = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

// Create Min Heap
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Swap
void swapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Heapify
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < (int)minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < (int)minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Store Huffman codes
void storeCodes(struct MinHeapNode* root, int arr[], int top, char codes[MAX_CHAR][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }

    if (isLeaf(root)) {
        for (int i = 0; i < top; i++)
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        codes[(unsigned char)root->data][top] = '\0';
    }
}

// Calculate frequency
void calculateFreq(const char *text, int freq[]) {
    for (int i = 0; text[i] != '\0'; i++)
        freq[(unsigned char)text[i]]++;
}

// Write compressed file
void writeCompressed(const char *text, char codes[MAX_CHAR][MAX_TREE_HT]) {
    FILE *fp = fopen("output.huf", "wb");
    if (!fp) {
        printf("Error creating output file!\n");
        return;
    }

    fprintf(fp, "HEADER_START\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (strlen(codes[i]) > 0)
            fprintf(fp, "%c %s\n", i, codes[i]);
    }
    fprintf(fp, "HEADER_END\n");

    unsigned char buffer = 0;
    int bitCount = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        char *code = codes[(unsigned char)text[i]];
        for (int j = 0; code[j] != '\0'; j++) {
            buffer = (buffer << 1) | (code[j] - '0');
            bitCount++;
            if (bitCount == 8) {
                fwrite(&buffer, sizeof(unsigned char), 1, fp);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, sizeof(unsigned char), 1, fp);
    }
    fclose(fp);
    printf("\nCompressed file saved as output.huf\n");
}

// Decode file
void decompressFile(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error opening .huf file!\n");
        return;
    }

    char codes[MAX_CHAR][MAX_TREE_HT] = {0};
    char line[256];
    int readingHeader = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "HEADER_START", 12) == 0) {
            readingHeader = 1;
            continue;
        }
        if (strncmp(line, "HEADER_END", 10) == 0)
            break;
        if (readingHeader && line[1] == ' ') {
            char ch = line[0];
            char code[256];
            sscanf(line + 2, "%s", code);
            strcpy(codes[(unsigned char)ch], code);
        }
    }

    // Reverse lookup
    char reverse[MAX_CHAR][MAX_TREE_HT];
    for (int i = 0; i < MAX_CHAR; i++)
        if (strlen(codes[i]) > 0)
            strcpy(reverse[i], codes[i]);

    printf("\nDecompressed Text:\n");
    unsigned char buffer;
    char bitString[10000] = "";
    while (fread(&buffer, sizeof(unsigned char), 1, fp)) {
        for (int i = 7; i >= 0; i--) {
            char bit = ((buffer >> i) & 1) + '0';
            strncat(bitString, &bit, 1);
        }
    }

    char temp[256] = "";
    for (int i = 0; i < (int)strlen(bitString); i++) {
        strncat(temp, &bitString[i], 1);
        for (int j = 0; j < MAX_CHAR; j++) {
            if (strlen(reverse[j]) > 0 && strcmp(temp, reverse[j]) == 0) {
                printf("%c", j);
                temp[0] = '\0';
                break;
            }
        }
    }
    printf("\n\nDecompression Complete!\n");
    fclose(fp);
}

// Compare storage bits
void compareStorage(const char *text, int uniqueChars, char codes[MAX_CHAR][MAX_TREE_HT]) {
    int asciiBits = strlen(text) * 8;
    int fixedBits = strlen(text) * (int)ceil(log2(uniqueChars));
    int huffmanBits = 0;
    for (int i = 0; text[i] != '\0'; i++)
        huffmanBits += strlen(codes[(unsigned char)text[i]]);

    printf("\n\n===== STORAGE COMPARISON =====\n");
    printf("ASCII encoding (8 bits per char): %d bits\n", asciiBits);
    printf("Fixed-length code: %d bits\n", fixedBits);
    printf("Huffman encoding: %d bits\n", huffmanBits);
    printf("Compression ratio: %.2f%%\n", (100.0 * huffmanBits / asciiBits));
}

int main() {
    char filename[100];
    printf("Enter text filename to compress: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    char text[10000];
    int idx = 0;
    while ((text[idx] = fgetc(fp)) != EOF) idx++;
    text[idx] = '\0';
    fclose(fp);

    int freq[MAX_CHAR] = {0};
    calculateFreq(text, freq);

    char arr[MAX_CHAR];
    int freqArr[MAX_CHAR], size = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i]) {
            arr[size] = (char)i;
            freqArr[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(arr, freqArr, size);
    char codes[MAX_CHAR][MAX_TREE_HT] = {{0}};
    int huffmanArr[MAX_TREE_HT], top = 0;
    storeCodes(root, huffmanArr, top, codes);

    printf("\nGenerated Huffman Codes:\n");
    for (int i = 0; i < MAX_CHAR; i++)
        if (strlen(codes[i]) > 0)
            printf("%c: %s\n", i, codes[i]);

    // Part A: Compare storage
    compareStorage(text, size, codes);

    // Part B: Write compressed file
    writeCompressed(text, codes);

    // Part C: Decompress file
    printf("\n\nNow decompressing the file...\n");
    decompressFile("output.huf");

    return 0;
}
