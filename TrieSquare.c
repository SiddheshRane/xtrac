
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "TrieSquare.h"
#include "TokenSet.h"
#include "Huffman.h"
#include "list.h"

typedef struct Trie {
    unsigned count[256];
} Trie;

typedef struct TrieSquare {
    Trie *ascii[256];
    int count[256];
} TrieSquare;

typedef struct Key {
    int i;
    int j;
} Key;


static TrieSquare trisqr = {
    {NULL},
    {0}
};

void addChar(int prev, int new) {
    if (!trisqr.ascii[prev]) {
        trisqr.ascii[prev] = calloc(1, sizeof (Trie));
    }
    trisqr.ascii[prev]->count[new]++;
}

void printChar(int ch) {
    switch (ch) {
        case ' ': printf("' '");
            return;
        case '\n': printf("\\n");
            return;
        case '\t': printf("\\t");
            return;
        case '\f': printf("\\f");
            return;
    }
    if (isprint(ch))
        printf("%c", ch);
    else
        printf("%2d", ch);
}

void printTrieSquare() {
    int i, j = 0;
    unsigned char unused[256] = {0};
    for (i = 0; i < 256; i++) {
        Trie* trie = trisqr.ascii[i];
        if (trie) {
            printf("%3d: (%d|", i, trisqr.count[i]);
            printChar(i);
            printf(") -> ");
            int j;
            for (j = 0; j < 256; j++) {
                if (trie->count[j] > 0) {
                    printf("(%d|", trie->count[j]);
                    printChar(j);
                    printf(") ");
                }
            }
            printf("\n");
        } else {
            unused[j++] = i;
        }
    }
    printf("Unused Characters(%d)\n[", j);
    for (i = 0; i < j; i++) {
        printChar((unsigned) unused[i]);
        printf("|");
    }
    printf("]");
    HuffmanTree *hf = buildHuffmanTree(trisqr.count);

}

static int compareTrie(void* v1, void* v2) {
    if (!v1 || !v2) {
        printf("compareTrie(NULL)\n");
        return 0;
    }
    Key *k1 = (Key*) v1;
    Key *k2 = (Key*) v2;

    unsigned c1 = trisqr.ascii[k1->i]->count[k1->j];
    unsigned c2 = trisqr.ascii[k2->i]->count[k2->j];

    if (c1 == c2)
        return 0;
    if (c1 > c2) {
        return 1;
    }
    return -1;

}

list* sortTrie() {
    list *l = newList();
    list_setComparator(l, compareTrie);
    int i;
    for (i = 0; i < 256; i++) {
        if (!trisqr.ascii[i]) {
            continue;
        }
        int j;
        for (j = 0; j < 256; j++) {
            if (trisqr.ascii[i]->count[j] < 2U)
                continue;
            Key *k = calloc(1, sizeof (Key));
            k->i = i;
            k->j = j;
            list_append(l, k);
        }
    }
    return l;
}

void printSortedTrie(list* l) {
    if (!l || list_count(l) < 1) {
        printf("List empty");
        return;
    }
    /*A temporary hack that directly accesses the linked list */
    list_node* node = l->head;
    while (node) {
        Key *k = (Key*) node->value;
        printf("%4u:", trisqr.ascii[k->i]->count[k->j]);
        printChar(k->i);
        printChar(k->j);
        printf("\n");
        node = node->next;
    }
    printf("%d number of abbreviations\n", l->count);
}

void parseIntoTrySquare(FILE *input) {
    if (!input) {
        return;
    }
    int ch = 0;
    int prev = 0;
    while ((ch = fgetc(input)) != -1) {
        addChar(prev, ch);
        trisqr.count[ch]++;
        prev = ch;
    }
}

int TrieSquare_main(int argc, char**argv) {
    if (argc < 2) {
        printHelp();
        return 1;
    }
    FILE *ifile = fopen(argv[1], "r");
    if (!ifile) {
        printf("Could not open file %s", argv[1]);
        return 1;
    }
    parseIntoTrySquare(ifile);
    printTrieSquare();
    printf("\nsortingTrie\n");
    list* sortedList = sortTrie();
    printf("printing Sorted Trie\n");
    printSortedTrie(sortedList);
    fclose(ifile);
    return 0;
}
