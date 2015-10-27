
#include <stdio.h>
#include <stdlib.h>

typedef struct Prefix {
    char ascii;
    struct Prefix* zero, *one;
} Prefix;

typedef struct HuffmanTree {
    Prefix *root;
    int maxHeight;
} HuffmanTree;

int compare(void* i1, void* i2) {
    int a = *((int*) i1);
    int b = *((int*) i2);
    if (a == b)
        return 0;
    if (a > b) {
        return 1;
    }
    return -1;

}

HuffmanTree* buildHuffmanTree(int count[256]) {
    int copy[256];
    int i;
    for (i = 0; i < 256; i++) {
        copy[i] = count[i];
    }
    qsort(copy, 256, sizeof (int), compare);
    HuffmanTree hf;
}