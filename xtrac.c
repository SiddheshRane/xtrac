#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "xtrac.h"

////////////////////////TOKEN RELATED///////////////////

//Standard Pointers to Tokens
Token *START = NULL, *END = NULL;
//the no. of tokens
int tokenCount = 0;
//character map
int charmap[256];
long numChars = 0; //total no. of characters read
int numFree = 0; //no. of unused characters
int MAX_GAIN = 0;
//file pointer to last read position in input file
long lastRead = 0;
long ifs = 1; //input file size

char *INPUTFILE = "taylor.txt";
char *OUTPUTFILE = "output.xtr";
FILE *ifile = NULL;
FILE *ofile = NULL;

//important

/*
Analyses a string to see if it matches
any of the existing tokens. If a match is found
it increments the token's count else it creates
a new token.
Returns : 0 for successful termination
          1 for unsuccessful termination due to
          being out of memory.

 */
int append(char * word) {
    Token *ptr = START;
    Token *last = NULL;
    while (ptr) {
        //increment count if token exists
        int match = strcmp(word, ptr->token);
        if (match == 0) {
            ptr->gain += ptr->size - 2;
            return 0;
        }//check where to append
        else if (match < 0) {
            break; //tokens ahead wont match so stop
        }

        last = ptr;
        ptr = ptr->next;
    }//end of while

    //append element if it does not exist
    Token *temp = malloc(sizeof (Token));
    if (!temp) return 1; //indicateOutOFMemory and exit
    int len = strlen(word) + 1;
    temp->token = malloc(len);
    if (!temp->token) return 1;
    strcpy(temp->token, word);
    temp->gain = -3; //when c=1 gain = c(s-2)-(s+1) = -3
    temp->size = len;
    temp->code = 0; //0 means unassigned
    temp->next = NULL;

    if (len <= 1) {
        free(temp->token);
        free(temp);
    } else
        appendToken(temp, last);
    return 0;
}//end of append

/*
insert 'token' after 'after'
 */
void appendToken(Token *token, Token *after) {
    //cout<<"\nappending token..";
    //warn("Warning" , "appending token");
    if (after) {
        Token *temp = after->next;
        after->next = token;
        token->next = temp;
    } else if (!START) { //nothing has been stored
        START = END = token;
    } else { //append at first position
        token->next = START;
        START = token;
    }
    tokenCount++;
}

/*This deletes the Token
after prev and NOT prev

 */
int del(Token* prev) {
    Token *ptr;

    if (!prev) { //delete START
        ptr = START;
        if (!START)
            return 0;
        START = START->next;
        free(ptr->token);
        free(ptr);
        tokenCount--;
        return 1;
    }
    ptr = prev->next;
    if (ptr) {
        prev->next = ptr->next;
        free(ptr->token);
        free(ptr);
        tokenCount--;
        return 1;
    }
    return 0; //no deletion occured
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
        }//end for
        fprintf(omap, "</table>");
        numFree--; //one opcode is reserved
    }//end if
}

void strset(char *s, char c) {
    if (!s) return;
    while (*s) {
        *s++ = c;
    }
}

void printHelp() {
    printf("Usage:\nxtrac <inputfile> <outputfile>");
}

void printError(char *msg) {
    printf("%s", msg);
}
//-----------MAIN--------------

int xtrac_main(int argc, char **argv) {
    printf("Hope it reaches her\n");
    if (argc < 2) {
        printHelp();
        return 1;
    }
    ifile = fopen(argv[1], "r");
    if (!ifile) {
        printf("Could not open file %s", argv[1]);
        return 1;
    }
    ofile = fopen("XTR.xtr", "w");
    if (!ofile) {
        printf("Could not create output file");
        return 1;
    }

    START = END = NULL;
    tokenCount = 0;
    MAX_GAIN = 0;
    numFree = 0;

    //init charmap
    int i = 0;
    for (; i < 256; i++)charmap[i] = 0;
    printf("Parsing..\n");
    parseTest(ifile);
    printf("Charmap...\n");
    xmlCharmap();
    printf("traversing...\n");
    traverse();
    printf("Xtrac...\n");
    Xtrac();
    fclose(ifile);
    fclose(ofile);
}

//-----------IMPORTANT------------

void traverse() {
    //working
    Token *ptr = START, *last = NULL;
    while (ptr) {
        //memory clean up
        int gain = ptr->gain;
        if (gain < 1) {//no use of these tokens
            del(last);
            if (last) ptr = last->next;
            else {
                ptr = START;
                last = NULL;
            }
        } else {
            if (gain > MAX_GAIN)MAX_GAIN = gain;
            last = ptr;
            ptr = ptr->next;
        }
    }//end while
}



//FILE PARSE TEST

void parseTest(FILE *stream) {
    if (!stream) return;

    char buffer[64];
    char ch;
    int i = 0; //index to check buffer overflow
    ch = fgetc(stream);
    while (1) {
        buffer[i] = ch;
        charmap[ch]++; //update character map
        numChars++; //update no. of characters
        if (!isalnum(ch)) {
            buffer[i] = '\0';
            i = -1;
            int appSTAT = append(buffer);
            strset(buffer, '\0');
            if (appSTAT == 1)
                return;
        }//end if
        if ((ch = fgetc(stream)) == EOF)
            break;
        i++;
    }//while
    buffer[i] = '\0';
    append(buffer);
}//parseTest
//FILE PARSE TEST OVER


//Xtrac

/*
The most important method. It creates the compressed
file
 */
void Xtrac() {
    if (!ifile)return;
    if (!ofile) {
        exit(0);
    }
    printf("profiling...\n");
    profile();
    fputc(OP, ofile); //begin metadata
    //metadata
    {
        Token *ptr = START;
        while (ptr) {//there is a sentinel element we need to delete
            if (ptr->gain == MAX_GAIN)del(ptr);
            fputc(ptr->code, ofile);
            fprintf(ofile, "%s", ptr->token);
            fputc(OP, ofile);
            ptr = ptr->next;
        }
        //end metadata
        fputc(OP, ofile);
    }//end meta data
    //substitution
    {
        fseek(ifile, 0, 0);
        char buf[40], ch;
        strset(buf, '\0');
        while ((ch = fgetc(ifile)) != EOF) {
            if (!isalnum(ch))
                fputc(ch, ofile);
            else {
                int j = 1;
                buf[0] = ch;
                while (buf[j] = fgetc(ifile)) {
                    if (!isalnum(buf[j])) {//search for match;
                        ch = buf[j];
                        buf[j] = '\0';
                        Token *ptr = START;
                        while (ptr) {
                            if (!strcmp(ptr->token, buf)) {
                                fputc(ptr->code, ofile);
                                j = -1;
                                break;
                            }
                            ptr = ptr->next;
                        }//end while
                        if (j != -1) {
                            fprintf(ofile, "%s", buf);
                            strset(buf, '\0');
                        }
                        fputc(ch, ofile);
                        break;
                    }//end if
                    j++;
                }//end while
            }//end else
        }
    }
    Token *ptr = START;
    while (del(ptr));
}//Xtrac OVER

void profile() {
    Token *sentinel = malloc(sizeof (Token));
    if (sentinel) {
        sentinel->token = malloc(1);
        *(sentinel->token) = '\0';
        sentinel->size = 0;
        sentinel->code = 0;
        sentinel->next = START;
        sentinel->gain = -1;
        START = sentinel;
    }


    Token* ptr = START, *tokenmax = NULL; //tokenmax is token before largest token
    Token *origSTART = START;

    /*
    out of numFree and tokenCount we choose whichever is smaller.
    Otherwise there will be infinite loop if no. of tokens is less than
    numFree
     */
    numFree = numFree < tokenCount ? numFree : tokenCount;
    //find the largest token
    int j;
    for (j = 0; j < numFree; j++) {
        int max = ptr->gain;
        while (ptr->next) {
            if (ptr->next->gain > max) {
                max = ptr->next->gain;
                tokenmax = ptr;
            }
            ptr = ptr->next;
        }
        //make the largest token the first token
        if (tokenmax) {
            //if tokenmax is not null
            Token *temp = tokenmax->next;
            tokenmax->next = temp->next;
            temp->next = START;
            START = temp;
            ptr = origSTART;
            tokenmax = NULL;
        }
    }
    //now assign codes
    ptr = START;
    int i;
    for (i = 1; i < 256; i++) {
        if (charmap[i] == 0) {
            if (!ptr)break;
            ptr->code = i;
            ptr = ptr->next;
        }
    }
    ptr = origSTART;

    while (origSTART->next != NULL) {
        del(ptr);
    }
    traverse();
}

