/* 
 * File:   xtrac.h
 * Author: siddhesh
 *
 * Created on 24 October, 2015, 9:24 PM
 */
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
        unsigned char code; //the keycode for compression(0 means unassigned)
} Token;

    int del(Token *);
    void fileParse();
    void parseFile(FILE*);
    void Xtrac();
    void assignCodes();
    void removeIncompressibleTokens();

#ifdef	__cplusplus
}
#endif

#endif	/* XTRAC_H */

