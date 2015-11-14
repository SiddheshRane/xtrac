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

//character map
int charmap[256];
long numChars = 0; //total no. of characters read
int numFree = 0; //no. of unused characters

char *INPUTFILE;
char *OUTPUTFILE;
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
    if (!temp->token) {
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

void strset(char *s, char c) {
    if (!s) return;
    while (*s) {
        *s++ = c;
    }
}



//-----------MAIN--------------

int xtrac_main(FILE *input, FILE *output) {
    ifile = input;
    ofile = output;
    initGlobalTokenList();
    parseFile(ifile);
    removeIncompressibleTokens();
    xmlCharmap();
    assignCodes();
    Xtrac();
    fclose(ifile);
    fclose(ofile);
}

//-----------IMPORTANT------------

void removeIncompressibleTokens() {
    iterator* it = listGetIterator(&tokenList);
    Token* token;
    while ((token = listGetNext(it))) {
        if (token->gain < 1) {
            listDelete(&tokenList, token);
        }
    }
    free(it);
}

void parseFile(FILE *stream) {
    if (!stream) return;

    char buffer[64];
    int i = 0; //index to check buffer overflow
    int ch = fgetc(stream);
    while ((ch = fgetc(stream)) != EOF) {
        buffer[i] = ch;
        charmap[ch]++; //update character map
        numChars++; //update no. of characters
        if (!isalnum(ch)) {
            buffer[i] = '\0';
            i = -1;
            int appSTAT = appendNewWord(buffer);
            strset(buffer, '\0');
            if (appSTAT == 1)
                return;
        }
        i++;
    }
    buffer[i] = '\0';
    appendNewWord(buffer);
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
    char buf[40];
    int ch;
    strset(buf, '\0');
    while ((ch = fgetc(ifile)) != EOF) {
        if (!isalnum(ch)) {
            fputc(ch, ofile);
            continue;
        }
        int j = 1;
        buf[0] = ch;
        while (ch = fgetc(ifile)) {
            if (isalnum(ch)) {
                //Fill buf as long as you get alpha numeric
                buf[j] = ch;
                j++;
                continue;
            }
            //search for match;
            buf[j] = '\0';
            iterator* it = listGetIterator(&tokenList);
            Token* token;
            while ((token = listGetNext(it))) {
                if (strcmp(buf, token->token) == 0) {
                    fputc(token->code, ofile);
                    j = -1;
                }
            }
            if (j != -1) {
                fprintf(ofile, "%s", buf);
                strset(buf, '\0');
            }
            if (ch != EOF) {
                fputc(ch, ofile);
            }
            break;
        }
    }
    Token* tok;
    while ((tok = listDeleteHead(&tokenList))) {
        free(tok);
    }
}
static void printToken(void *t1) {
    Token *t = t1;
    printf("%d: (%d bytes) %s\n", t->code, t->gain, t->token);
}
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
    /*Debug*/
    printf("Printing Sorted Token List\n");
    listPrint(&sortedByGain,printToken);
}

