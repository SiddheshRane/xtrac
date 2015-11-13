/* 
 * File:   TrySquare.h
 * Author: siddhesh
 *
 * Created on 22 October, 2015, 7:46 PM
 */

#ifndef TRYSQUARE_H
#define	TRYSQUARE_H

#ifdef	__cplusplus
extern "C" {
#endif

    void parseIntoTrySquare(FILE* input);
    void printTrieSquare();
    list* getAllFeasibleCombinations(int i, int j);
#ifdef	__cplusplus
}
#endif

#endif	/* TRYSQUARE_H */

