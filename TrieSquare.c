
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "Huffman.h"
#include "list.h"
#include "xtrac.h"
#include "TrieSquare.h"

typedef struct Trie {
    unsigned count[256];
} Trie;

typedef struct TrieSquare {
    Trie* ascii[256];
    int count[256];
} TrieSquare;

typedef struct Key {
    int i;
    int j;
} Key;

typedef struct Repetitive {
    char* word;
    int freq;
    struct Repetitive* next;
} Repetitive;

typedef struct Graph {
    Repetitive* repetitives[256];
} Graph;

typedef struct SubString {
    char i, j;
    int freq;
    struct SubString *prefix, *suffix;
    struct SubString *sibling, *parent;
} SubString;

typedef struct StringGraph {
    SubString* graph[256];
} StringGraph;

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
        printf("%02d", ch);
}

void parseIntoTrySquare(FILE* input) {
    if (!input || feof(input)) {
        return;
    }
    int ch = 0;
    int prev = fgetc(input);
    while ((ch = fgetc(input)) != -1) {
        addChar(prev, ch);
        trisqr.count[ch]++;
        prev = ch;
    }

}

void printTrieSquare() {
    int i, j = 0;
    unsigned char unused[256] = {0};
    unsigned unusedUnicodes = 0;
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
                } else {
                    unusedUnicodes++;
                }
            }
            printf("\n");
        } else {
            unused[j++] = i;
            unusedUnicodes += 256;
        }
    }
    printf("Unused Characters(%d). Unused Unicodes(%u)\n[", j, unusedUnicodes);
    /*   for (i = 0; i < j; i++) {
           printChar((unsigned) unused[i]);
           printf("|");
       }
       printf("]");*/
    getchar();
    HuffmanTree *hf = buildHuffmanTree(trisqr.count);
    getchar();
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
    if (c1 < c2) {
        return 1;
    }
    return -1;

}

list* sortTrie() {
    list *l = newList();
    listSetComparator(l, compareTrie);
    int i;
    for (i = 0; i < 256; i++) {
        if (!trisqr.ascii[i]) {
            continue;
        }
        int j;
        for (j = 0; j < 256; j++) {
            if (trisqr.ascii[i]->count[j] < 2)
                continue;
            Key *k = calloc(1, sizeof (Key));
            k->i = i;
            k->j = j;
            listAppend(l, k);
        }
    }
    return l;
}

void printSortedTrie(list* l) {
    if (!l || listCount(l) < 1) {
        printf("List empty");
        return;
    }
    /*A temporary hack that directly accesses the linked list */
    list_node* node = l->head;
    int netCompression = 0;
    while (node) {
        Key *k = (Key*) node->value;
        int compression = trisqr.ascii[k->i]->count[k->j] - 4;
        if (compression > 1) {
            printf("%4u:", trisqr.ascii[k->i]->count[k->j]);
            printChar(k->i);
            printChar(k->j);
            printf("(%d bytes)\n", compression);
            netCompression += compression;
        }
        node = node->next;
    }
    printf("%d number of abbreviations\nNet Compression:%d", l->count, netCompression);
}

Repetitive* getElseAppend(Graph *g, char *word) {
    if (!g || !word) {
        return NULL;
    }
    static int counter = 1;
    Repetitive *match = g->repetitives[*word];
    Repetitive **appendHere = &g->repetitives[*word];
    while (match) {
        int compare = strcmp(match->word, word);
        if (compare == 0) {
            /*match found*/
            return match;
        } else if (compare > 0) {
            /*Further words wont match as the list is sorted*/
            break;
        }
        appendHere = &match->next;
        match = match->next;
    }
    printf("%d|%s|\n", counter++, word);
    Repetitive *toAppend = calloc(1, sizeof (Repetitive));

    toAppend->word = malloc(strlen(word));
    strcpy(toAppend->word, word);
    *appendHere = toAppend;
    return toAppend;
}

/*Probabilistic Model*/
list* generateMostProbableWords() {
    list *sortedByGain = newList();
    listSetComparator(sortedByGain, compareGainInverted);
    list *l;
    int i;
    for (i = 0; i < 256; i++) {
        Trie *trie = trisqr.ascii[i];
        if (!trie) {
            continue;
        }
        int j;
        for (j = 0; j < 256; j++) {
            l = getAllFeasibleCombinations(i, j);
            if (!l) {
                continue;
            }
            Token* tok;
            while ((tok = listDeleteHead(l))) {
                listAppend(sortedByGain, tok);
            }
        }
    }
    return sortedByGain;
}

list* getAllFeasibleCombinations(int i, int j) {
    Trie* trie = trisqr.ascii[i];
    if (!trie) {
        return NULL;
    }
    Trie* suffix = trisqr.ascii[j];
    if (!suffix) {
        return NULL;
    }
    int ijFreq = trie->count[j];
    if (ijFreq < 3) {
        /*Not feasible for compression*/
        return NULL;
    }
    list *l = newList();
    if (!l) {
        return NULL;
    };
    int freq = 0, gain = ijFreq - 4;
    int k;
    for (k = 0; k < 256; k++) {
        freq = suffix->count[k] > ijFreq ? ijFreq : suffix->count[k];
        gain = freq * 2 - 5;
        if (gain >= ijFreq - 4 && gain > 0) {
            char *word = malloc(4);
            word[0] = i;
            word[1] = j;
            word[2] = k;
            word[3] = '\0';
            Token *token = calloc(1, sizeof (Token));
            token->token = word;
            token->gain = gain;
            listAppend(l, token);
            //trisqr.ascii[j]->count[k]--;
        }
    }
    return l;
}

void printStringInList(void *v) {
    Token* tok = v;
    static int counter = 0;
    printf("<%d|%s>", tok->gain, tok->token);
    if (counter++ % 10 == 0) {
        printf("\n");
    }

}

void printAllFeasibleCombinations(list *l) {
    if (!l) {
        printf("List null\n");
    }
    printf("\nFeasible Combinations\n");
    listPrint(l, printStringInList);
    printf("\nTotal %d combinations\n", l->count);
}

/*StringGraph Probabilistic Model*/

SubString* newSubString() {
    SubString *ret = calloc(1, sizeof (SubString));
    return ret;
}

StringGraph* generateSubStringGraph() {
    StringGraph *graph = calloc(1, sizeof (StringGraph));
    int i;
    for (i = 0; i < 256; i++) {
        Trie *t = trisqr.ascii[i];
        if (!t) {
            continue;
        }
        int j;
        for (j = 0; j < 256; j++) {
            int freq = t->count[j];
            if (freq < 4) {
                continue;
            }
            SubString* sub = newSubString();
            sub->i = i;
            sub->j = j;
            sub->freq = freq;
            sub->sibling = graph->graph[i];
            graph->graph[i] = sub;
        }
    }
    return graph;
}

void printSubStringGraph(StringGraph* graph) {
    if (!graph) {
        printf("StringGraph Empty\n");
        return;
    }
    printf("\nStringGraph\n");
    int i;
    for (i = 0; i < 256; i++) {
        SubString* sub = graph->graph[i];
        if (!sub) {
            continue;
        }
        printf("%d|", sub->freq);
        printChar(sub->i);
        printChar(sub->j);
        printf("\n");
    }

}

SubString* findSubString(StringGraph* g, int i, int j) {
    if (!g) {
        return NULL;
    }

    SubString *ret = g->graph[i];
    while (ret) {
        if (ret->j == j) {
            return ret;
        }
        ret = ret->sibling;
    }
    return NULL;
}

SubString* getNextPossibleSubString(SubString *sub) {
    if (!sub) {
        return NULL;
    }
    int i = sub->i, j = sub->j;


}

/*Graph*/
Graph* parseIntoGraph(FILE *file) {
    if (!file || feof(file)) {
        return NULL;
    }
    Graph *graph = calloc(1, sizeof (Graph));
    const int threshold = 5;
    int ch, index = 0;
    int prev = fgetc(file);
    char buf[128];
    int freq = INT_MAX;
    int prevGain = 0;
    Repetitive *prevWord = NULL;
    while ((ch = fgetc(file)) != EOF) {
        if (trisqr.ascii[prev]->count[ch] < threshold) {
            index = 0;
            freq = INT_MAX;
            prevGain = 0;
            prevWord = NULL;
            prev = ch;
            continue;
        }
        freq = trisqr.ascii[prev]->count[ch] < freq ? trisqr.ascii[prev]->count[ch] : freq;
        buf[index] = prev;
        buf[index + 1] = ch;
        buf[index + 2] = '\0';
        index++;
        if (freq * (index) - (index + 3) > prevGain && !(prevWord && prevWord->freq < 2)) {
            prevWord = getElseAppend(graph, buf);
            prevWord->freq++;
            prevGain = freq * (index) - (index + 3);
        } else {
            /*No use continuing further. Break this word and start a new one*/
            index = 0;
            freq = INT_MAX;
            prevWord = NULL;
            prevGain = 0;
        }
        trisqr.ascii[prev]->count[ch]--;
        prev = ch;
    }
    return graph;

}

void printGraph(Graph *g) {
    if (!g) {
        printf("Graph empty\n");
        return;
    }
    printf("\n---Printing Graph---\n");
    Repetitive *rep;
    int i;
    for (i = 0; i < 256; i++) {
        rep = g->repetitives[i];
        if (!rep) {
            continue;
        }
        printf("%3d:", i);
        printChar(i);
        printf("\n");
        while (rep) {
            printf("%5d|%s|\n", rep->freq, rep->word);
            rep = rep->next;
        }
        printf("\n");
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

    /*
        struct list* probable = generateMostProbableWords();
        printAllFeasibleCombinations(probable);
     */
    /*

            StringGraph* subgraph = generateSubStringGraph();
            printSubStringGraph(subgraph);
     */
    fseek(ifile, 0, 0);
    Graph* graph = parseIntoGraph(ifile);
    printGraph(graph);
    fclose(ifile);
    return 0;
}