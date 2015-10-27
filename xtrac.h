/* 
 * File:   xtrac.h
 * Author: siddhesh
 *
 * Created on 24 October, 2015, 9:24 PM
 */
#include <stdio.h>
#ifndef XTRAC_H
#define	XTRAC_H

#ifdef	__cplusplus
extern "C" {
#endif
#define OP 0
typedef struct Token {
    char * token;
    int size; //length of token obtained using strlen + 1
    int gain; //compressional gain (=(size-2)*count - (size+1))
    char code; //the keycode for compression(0 means unassigned)
    struct Token * next; //for linked list
} Token;

int append(char*);
void appendToken(Token*, Token*);
int del(Token *);
void traverse();
void console();
void fileParse();
void strset(char*, char);
void parseTest(FILE*);
void Xtrac();
void profile();


#ifdef	__cplusplus
}
#endif

#endif	/* XTRAC_H */
