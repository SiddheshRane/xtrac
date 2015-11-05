#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#define OP 0
#define VERSION 1

static char* map[256];

static int tokenCount = 0;

int dextr(FILE* input, FILE* output);
void printMap();

long filesize;

static void load_map(FILE* input) {
    if (!input || feof(input)) {
        return;
    }
    if (fgetc(input)) {
        //if not zero, no map is defined in input file
        fseek(input, -1, SEEK_CUR);
        return ;
    }

    int key;
    size_t len;
    while ((key = fgetc(input)) != EOF && key) {
        char* value = NULL;
        getdelim(&value, &len, OP, input);
        map[key] = value;
    }

}

void decompress(FILE* input, FILE* output) {
    if (!input || !output) {
        return;
    }
    int buf;
    while ((buf = fgetc(input)) != EOF) {
        if (map[buf])
            fprintf(output, "%s", map[buf]);
        else
            fputc(buf, output);
    }
}

int dextr(FILE* input, FILE* output) {
    if (!input || !output) {
        return 1;
    }
    fseek(input, 0, 0);
    int buf = fgetc(input);
    if (buf != VERSION)
        return 2;
    load_map(input);
    printMap();
    decompress(input, output);
}

void printMap() {
    printf("Printing Map\n");
    int i;
    for (i = 0; i < 256; i++) {
        if(map[i]){
            printf("%3d:%s\n", i, map[i]);
        }
    }

}
///////////////MAIN//////////////////////

int dextr_main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage:\n dextr <inputfile> [<outputfile>]\n");
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Could not open input file");
        return 1;
    }
    
    FILE* output;
    if(argc > 2) {
        output = fopen(argv[2], "w");
    }
    else {
        int len = strlen(argv[1]);
        char* filename = malloc(len+4);
        strcpy(filename, argv[1]);
        filename[len] = 'T';
        filename[len+1] = 'X';
        filename[len+2] = 'T';
        output = fopen(filename, "w");
    }
    if (!output) {
        perror("Could not open output file");
        return 1;
    }
    int code = dextr(input, output);

    switch (code) {
        case 1:
        case 2:
        case 3:
            printf("Dextr error code %d. Press any key to exit", code);
    }
    fclose(input);
    fclose(output);
}
