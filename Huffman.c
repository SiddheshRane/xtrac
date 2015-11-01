
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#include "list.h"

Prefix* newPrefix() {
    Prefix *p = calloc(1, sizeof (Prefix));
    return p;
}

int comparePrefix(void* i1, void* i2) {
    int a = ((Prefix*) i1)->freq;
    int b = ((Prefix*) i2)->freq;
    if (a == b)
        return 0;
    if (a > b) {
        return 1;
    }
    return -1;
}

void printPrefix(void* value) {
    Prefix* p = (Prefix*) value;
    printf("(freq:%d|ascii:%c) ", p->freq, p->ascii);
}

HuffmanTree* buildHuffmanTree(int count[256]) {
    printf("buildingHuffmanTree\n");
    list l = initList;
    list_setComparator(&l, comparePrefix);
    int i;
    for (i = 0; i < 256; i++) {
        if (count[i] == 0) {
            continue;
        }

        Prefix *p = newPrefix();
        if (!p)
            return NULL;
        p->ascii = i;
        p->freq = count[i];
        printf("(freq:%d) ", p->freq);
        list_append(&l, p);
    }

    list_print(&l, printPrefix);
    printf("\n");
    Prefix *p1, *p2;
    while ((p1 = list_delete_head(&l)) && (p2 = list_delete_head(&l))) {
        Prefix *newP = newPrefix();
        newP->freq = p1->freq + p2->freq;
        if (p1->zero && p1->one) {
            newP->zero = p2;
            newP->one = p1;
        } else {
            newP->zero = p1;
            newP->one = p2;
        }
        list_append(&l, newP);
    }
    HuffmanTree *hf = calloc(1, sizeof (HuffmanTree));
    hf->root = p1;
    printHuffmanTree(hf);
    return hf;
}

void print_binary(unsigned long l, unsigned digits) {
    while(l || digits) {
        if(l & 1)
            printf("1");
        else
            printf("0");
        l = l >> 1;
        digits--;
    }
}

void print_prefix_tree(Prefix* p, unsigned level) {
    if (!p)
        return;
    if (!(p->one && p->zero)) {
        printf("->%d|",p->freq);
        printChar(p->ascii);
        return ;
    }
    printf("\n%*c", level, '0');
    print_prefix_tree(p->zero, level+1);
    printf("\n%*c", level, '1');
    print_prefix_tree(p->one, level+1);

}

void printHuffmanTree(HuffmanTree* hf) {
    Prefix *p = hf->root;
    print_prefix_tree(p, 1);
}


///////Huffman Main/////////

int Huffman_main(int argc, char** argv) {

}