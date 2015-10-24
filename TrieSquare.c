
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Trie {
    unsigned count[256];
} Trie;

typedef struct TrieSquare {
    Trie *ascii[256];
    int count[256];
} TrieSquare;

TrieSquare trisqr = {
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
    int i;
    for (i = 0; i < 256; i++) {
        Trie* trie = trisqr.ascii[i];
        if (trie) {
            printf("%3d: (%d|", i, trisqr.count[i]);
            printChar(i);
            printf(") -> ");
            int j;
            for (j = 0; j < 256; j++) {
                if (trie->count[j] > 1) {
                    printf("(%d|", trie->count[j]);
                    printChar(j);
                    printf(") ");
                }
            }
            printf("\n");
        }

    }

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
    fclose(ifile);
    return 0;
}