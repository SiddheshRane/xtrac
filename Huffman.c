
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
	Prefix *p = calloc(1, sizeof(Prefix));
	return p;
}

int comparePrefix(void* i1, void* i2) {
    int a = ((Prefix*)i1)->freq;
    int b = ((Prefix*)i2)->freq;
    if (a == b)
        return 0;
    if (a > b) {
        return 1;
    }
    return -1;
}

HuffmanTree* buildHuffmanTree(int count[256]) {
    list l = initList;
    list_setComparator(&l, comparePrefix);
    int i;
    for (i = 0; i < 256; i++) {
        Prefix *p = newPrefix();
        if(!p)
        	return NULL;
        p->ascii = i;
        p->freq = count[i];
        list_append(&l, p);
    }
    
    Prefix *p1, p2;
    while((p1 = list_delete_head(&l)) && (p2 = list_delete_head(&l))) {
    	Prefix *newP = newPrefix();
    	newP->freq = p1->freq + p2->freq;
    	newP->zero = p1;
    	newP->one = p2;
    	list_append(&l, newP);   	
    }
    HuffmanTree *hf = calloc(1, sizeof(HuffmanTree));
    p1 = p1 == NULL ? p2 : p1;
    hf->root = p1;
    return hf;
}
