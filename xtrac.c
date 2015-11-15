/* 
 * Copyright (C) 2015 Siddhesh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "xtrac.h"
#include "list.h"
#define VERSION 1
#define VERBOSE 1
#define BUFFER 128

//character map
int charmap[256];
unsigned flag = 0;
long numChars = 0; //total no. of characters read
int numFree = 0; //no. of unused characters
FILE *ifile;
FILE *ofile;
static list tokenList;

/*
 * A lexicographic comparator for string tokens inside Token
 */
static int compareToken(void* t1, void* t2) {
    Token *tt1 = t1;
    Token *tt2 = t2;
    if (!t1 && !t2)
        return 0;
    if (!t1)
        return -1;
    if (!t2) {
        return 1;
    }
    return strcmp(tt1->token, tt2->token);
}

/*
 * An inverted comparator to get a descending sorted list. 
 */
static int compareGainInverted(void*t1, void* t2) {
    Token *tt1 = t1;
    Token *tt2 = t2;
    int a = tt1->gain;
    int b = tt2->gain;
    if (a == b)
        return 0;
    if (a > b) {
        return -1;
    }
    return 1;
}

static void initGlobalTokenList() {
    tokenList = initList;
    listSetComparator(&tokenList, compareToken);
    listSetDistinctElements(&tokenList, 1);
}

int appendNewWord(char* word) {
    int len = strlen(word) + 1;
    if (len < 3) {
        //single letter words cannot be compressed
        return 0;
    }
    Token* temp = malloc(sizeof (Token));
    if (!temp) {
        return 1; //indicateOutOFMemory and exit
    }

    temp->token = malloc(len);
    if (!(temp->token)) {
        free(temp);
        return 1;
    }
    strcpy(temp->token, word);
    temp->gain = -3; //when c=1 gain = c(s-2)-(s+1) = -3
    temp->size = len;
    temp->code = 0; //0 means unassigned

    Token* appended = listAppend(&tokenList, temp);
    if (appended != temp) {
        appended->gain += appended->size - 2;
        free(temp->token);
        free(temp);
    }
    return 0;
}

void xmlCharmap() {
    FILE* omap = fopen("charEXCEL.xml", "w");
    if (omap) {
        fprintf(omap, "<table>");
        int j;
        for (j = 0; j < 256; j++) {
            if (charmap[j] == 0) numFree++; //increment unused char count
            fprintf(omap, "<tr><td>");
            fprintf(omap, "%c</td><td>%d</td><td>%d</td></tr>\n", j, j, charmap[j]);
        }
        fprintf(omap, "</table>");
        numFree--; //one opcode is reserved
        fclose(omap);
    }
}


/*
 *-----------MAIN--------------
 */

int xtrac_main(FILE *input, FILE *output, unsigned _flag) {
    ifile = input;
    ofile = output;
    flag = _flag;
    initGlobalTokenList();
    parseFile(ifile);
    removeIncompressibleTokens();
    if (flag & VERBOSE) {
        xmlCharmap();
    }
    assignCodes();
    Xtrac();
    fclose(ifile);
    fclose(ofile);
}
/*Removes words that don't occur frequently enough for compression*/
void removeIncompressibleTokens() {
    iterator* it = listGetIterator(&tokenList);
    Token* token;
    while ((token = listGetNext(it))) {
        if (token->gain < 1) {
            listDelete(&tokenList, token);
            free(token->token);
            free(token);
        }
    }
    free(it);
}

/*Tokenizes the input file with words delimited by punctuation*/
void parseFile(FILE *stream) {
    if (!stream) return;

    char buffer[BUFFER];
    int i = 0; //index to check buffer overflow
    int ch;
    while ((ch = fgetc(stream)) != EOF) {
        buffer[i] = ch;
        charmap[ch]++; //update character map
        numChars++; //update no. of characters
        if (!isalnum(ch)) {
            buffer[i] = '\0';
            i = -1;
            int appSTAT = appendNewWord(buffer);
            if (appSTAT == 1)
                return;
        }//end if
        i++;
        if (i == BUFFER) {
            i = 0;
        }
    }//while
    buffer[i] = '\0';
    appendNewWord(buffer);
}

Token* findMatch(char *word) {
    iterator* it = listGetIterator(&tokenList);
    Token *match;
    while ((match = listGetNext(it))) {
        if (strcmp(match->token, word) == 0) {
            return match;
        }
    }
    free(it);
    return NULL;
}

/*
 * The most important method. It creates the compressed file
 */
void Xtrac() {
    fputc(VERSION, ofile);
    if (listCount(&tokenList)) {
        /*Insert meta data if there is any*/
        fputc(OP, ofile); //begin meta data
        iterator* it = listGetIterator(&tokenList);
        Token *token;
        while ((token = listGetNext(it))) {
            fputc(token->code, ofile);
            fprintf(ofile, "%s", token->token);
            fputc(OP, ofile);
        }
        free(it);
        fputc(OP, ofile); //end meta data
    }
    //substitution
    fseek(ifile, 0, 0);
    char buffer[BUFFER] = {0};
    int i = 0; //index to check buffer overflow
    int ch;
    while ((ch = fgetc(ifile)) != EOF) {
        buffer[i] = ch;
        if (!isalnum(ch)) {
            if (i == 0) {
                fputc(ch, ofile);
                continue;
            } else if (i == 1) {
                /*one character can never be compressed*/
                fputc(buffer[0], ofile);
                fputc(ch, ofile);
                i = 0;
                continue;
            }
            buffer[i] = '\0';
            Token *tok = findMatch(buffer);
            if (tok) {
                fputc(tok->code, ofile);
                fputc(ch, ofile);
            } else {
                fwrite(buffer, 1, i, ofile);
                fputc(ch, ofile);
            }
            i = 0;
            continue;
        }
        i++;
        if (i == BUFFER) {
            fwrite(buffer, 1, BUFFER, ofile);
            i = 0;
        }
    }//while
    buffer[i] = '\0';
    Token* tok = findMatch(buffer);
    if (tok) {
        fputc(tok->code, ofile);
    }

    while ((tok = listDeleteHead(&tokenList))) {
        free(tok->token);
        free(tok);
    }
}

static void printToken(void *t1) {
    Token *t = t1;
    printf("%d: (%d bytes) %s\n", t->code, t->gain, t->token);
}
/*Assigns unused ascii codes to most repeating words*/
void assignCodes() {
    list sortedByGain = initList;
    listSetComparator(&sortedByGain, compareGainInverted);
    Token* token;
    while ((token = listDeleteHead(&tokenList))) {
        listAppend(&sortedByGain, token);
    }
    iterator* it = listGetIterator(&sortedByGain);
    int code;
    for (code = 1; code < 256; code++) {
        if (charmap[code] != 0) {
            continue;
        }
        token = listGetNext(it);
        if (!token) {
            break;
        }
        token->code = code;
    }
    //delete any remaining tokens
    while ((token = listDeleteNext(it))) {
        free(token);
    }
    tokenList = sortedByGain;
    free(it);
    if (flag & VERBOSE) {
        printf("Printing Sorted Token List\nCode (Bytes saved) Token\n");
        listPrint(&sortedByGain, printToken);
    }


}

