#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#define OP 0
#define VERSION 1

static char* map[256];
/*
static struct Token {
    char* token;
    char code; //the keycode for compression(0 means unassigned)
    struct Token *next; //for linked list

} *START = NULL, *END = NULL; */
/*
typedef struct Token Token;
 */
static int tokenCount = 0;

/*
static void append(Token*);
*/
int dextr(FILE* input, FILE* output);

long filesize;

static void load_map(FILE* input) {
    if (!input || feof(input)) {
        return;
    }
    if (fgetc(input)) {
        //if not zero, no map is defined in input file
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

//returns >0 on error , 0 on normal termination
/*
int dextr() {
    FILE* ifile = fopen(INPUTFILE, "r");


    if (!ifile) {
        perror("Could not open input file");
        return 1;
    }
    FILE* ofile = fopen(OUTPUTFILE, "w");
    if (!ofile) {
        perror("Could not open output file");
        return 2;
    }

    fseek(ifile, 0, 0);

    //version check
    char ch;
    ch = fgetc(ifile);
    if (ch != 1)return 3;

    //metadata
    ch = fgetc(ifile);
    if (ch == OP);
    char buf[128];
    ch = fgetc(ifile);
    size_t len;
    while (!feof(ifile)) {

        Token *tok = calloc(1, sizeof (Token));
        tok->code = ch;

        getdelim(&(tok->token), &len, 0, ifile);
        tok->next = NULL;
        append(tok);

        ch = fgetc(ifile);
        if (ch == OP)break;
    }


    while ((ch = fgetc(ifile)) != EOF) {

        Token *ptr = START;
        while (ptr) {
            if (ptr->code == ch) {
                if (!ptr->token) {
                    printf("token empty\n");
                    return 0;
                }
                fprintf(ofile, "%s", ptr->token);
                ch = OP;
                break;
            }
            ptr = ptr->next;
        }

        if (ch != OP)fputc(ch, ofile);
    }

    fclose(ifile);
    fclose(ofile);
    return 0;
}
 *?
/*
 append 'token' to end
 
static void append(Token *token) {
    if (END) {
        END->next = token;
        END = token;

    } else if (!START) { //nothing has been stored

        START = END = token;
    }
    tokenCount++;
}
 */

