/* 
 * File:   TokenList.h
 * Author: siddhesh
 *
 * Created on 25 October, 2015, 5:35 PM
 */

#ifndef TOKENSET_H
#define	TOKENSET_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct Token {
        char * token;
        int size; //length of token obtained using strlen + 1
        int gain; //compressional gain (=(size-2)*count - (size+1))
        char code; //the keycode for compression(0 means unassigned)
        struct Token *next; //for linked list
        struct Token *prev;
    } Token;

    typedef struct TokenSet {
        Token *head;
        Token *tail;
        int tokenCount;
        int (*compare)(void*, void*);
    } TokenSet;

    extern TokenSet initTokenSet;

#ifdef	__cplusplus
}
#endif
#endif	/* TOKENLIST_H */

