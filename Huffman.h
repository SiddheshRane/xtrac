typedef struct Prefix {
    int freq;
    char ascii;
    struct Prefix *zero, *one;
} Prefix;

typedef struct HuffmanTree {
    Prefix *root;
    int maxHeight;
} HuffmanTree;

HuffmanTree* buildHuffmanTree(int count[256]);
