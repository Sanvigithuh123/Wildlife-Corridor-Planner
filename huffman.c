
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "huffman.h"

#define SYMBOLS 256
#define MAX_NODES 512

typedef struct Node {
    unsigned char symbol;
    unsigned long long freq;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *nodes[MAX_NODES];
    int size;
} MinHeap;

static Node *createNode(
    unsigned char symbol,
    unsigned long long freq,
    Node *left,
    Node *right
) {
    Node *node = malloc(sizeof(Node));

    if (node == NULL)
        return NULL;

    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

static void freeTree(Node *root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

static void heapPush(MinHeap *heap, Node *node) {
    int i = heap->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (heap->nodes[parent]->freq <= node->freq)
            break;

        heap->nodes[i] = heap->nodes[parent];
        i = parent;
    }

    heap->nodes[i] = node;
}

static Node *heapPop(MinHeap *heap) {
    Node *result = heap->nodes[0];
    Node *last = heap->nodes[--heap->size];

    if (heap->size == 0)
        return result;

    int i = 0;

    while (2 * i + 1 < heap->size) {
        int child = 2 * i + 1;

        if (child + 1 < heap->size &&
            heap->nodes[child + 1]->freq <
            heap->nodes[child]->freq) {
            child++;
        }

        if (last->freq <= heap->nodes[child]->freq)
            break;

        heap->nodes[i] = heap->nodes[child];
        i = child;
    }

    heap->nodes[i] = last;
    return result;
}

static void generateCodes(
    Node *root,
    char path[],
    int depth,
    char codes[SYMBOLS][SYMBOLS]
) {
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL) {
        if (depth == 0)
            path[depth++] = '0';

        path[depth] = '\0';
        strcpy(codes[root->symbol], path);
        return;
    }

    if (root->left != NULL) {
        path[depth] = '0';
        generateCodes(
            root->left, path, depth + 1, codes
        );
    }

    if (root->right != NULL) {
        path[depth] = '1';
        generateCodes(
            root->right, path, depth + 1, codes
        );
    }
}

int huffmanCompress(const char *filename) {
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
        printf("Cannot open monitoring file.\n");
        return -1;
    }

    unsigned long long freq[SYMBOLS] = {0};
    unsigned long long totalBytes = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        freq[(unsigned char)ch]++;
        totalBytes++;
    }

    if (ferror(fp)) {
        printf("Error reading monitoring file.\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);

    if (totalBytes == 0) {
        printf("Monitoring file is empty.\n");
        return -1;
    }

    MinHeap heap = {.size = 0};

    for (int i = 0; i < SYMBOLS; i++) {
        if (freq[i] > 0) {
            Node *node = createNode(
                (unsigned char)i,
                freq[i],
                NULL,
                NULL
            );

            if (node == NULL) {
                printf("Memory allocation failed.\n");
                while (heap.size > 0)
                    freeTree(heapPop(&heap));
                return -1;
            }

            heapPush(&heap, node);
        }
    }

    while (heap.size > 1) {
        Node *left = heapPop(&heap);
        Node *right = heapPop(&heap);

        Node *parent = createNode(
            0,
            left->freq + right->freq,
            left,
            right
        );

        if (parent == NULL) {
            freeTree(left);
            freeTree(right);

            while (heap.size > 0)
                freeTree(heapPop(&heap));

            printf("Memory allocation failed.\n");
            return -1;
        }

        heapPush(&heap, parent);
    }

    Node *root = heapPop(&heap);

    char codes[SYMBOLS][SYMBOLS] = {{0}};
    char path[SYMBOLS];

    generateCodes(root, path, 0, codes);

    unsigned long long encodedBits = 0;

    printf("\n===== HUFFMAN CODING =====\n");
    printf("Symbol Frequencies and Codes:\n\n");

    for (int i = 0; i < SYMBOLS; i++) {
        if (freq[i] > 0) {
            printf(
                "Byte %3d | Frequency: %llu | Code: %s\n",
                i,
                freq[i],
                codes[i]
            );

            encodedBits +=
                freq[i] * strlen(codes[i]);
        }
    }

    unsigned long long originalBits = totalBytes * 8;

    printf("\nOriginal Size: %llu bits\n",
           originalBits);
    printf("Encoded Symbol Bits: %llu bits\n",
           encodedBits);

    printf(
        "Symbol Stream Reduction: %.2f%%\n",
        100.0 * (1.0 -
            (double)encodedBits / (double)originalBits)
    );

    freeTree(root);
    return 0;
}
