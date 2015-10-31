
#include <stdio.h>
#include <stdlib.h>

typedef struct Prefix {
    int freq;
    char ascii;
    struct Prefix *zero, *one;
} Prefix;

typedef struct HuffmanTree {
    Prefix *root;
    int maxHeight;
} HuffmanTree;

Prefix* newPrefix() {
	static Prefix initPrefix
	Prefix *p = malloc(sizeof(Prefix));
}

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
    int i;
    for (i = 0; i < 256; i++) {
        
    }
    
    HuffmanTree hf;
}
