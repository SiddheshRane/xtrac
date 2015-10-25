
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TokenSet.h"

TokenSet initTokenSet = {0, 0, 0};

int compareToken(void* t1, void* t2) {
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

int compareCount(void* t1, void* t2) {
    Token *tt1 = t1;
    Token *tt2 = t2;
    if (!t1 && !t2)
        return 0;
    if (!t1)
        return -1;
    if (!t2) {
        return 1;
    }
    if (tt1->gain == tt2->gain)
        return 0;
    if (tt1->gain > tt2->gain) {
        return 1;
    }
    return -1;
}

TokenSet *newTokenSet() {
    TokenSet *ret = malloc(sizeof (TokenSet));
    if (!ret) {
        return NULL;
    }
    ret->head = ret->tail = NULL;
    ret->compare = NULL;
    ret->tokenCount = 0;
    return ret;
}

Token *newToken() {
    Token *ret = malloc(sizeof (Token));
    if (!ret) {
        return NULL;
    }
    ret->code = 0;
    ret->gain = -1;
    ret->next = ret->prev = NULL;
    ret->token = NULL;
    ret->size = 0;
    return ret;
}

void setComparator(TokenSet* tset, int (*com)(void*, void*)) {
    if (!tset)
        return;
    tset->compare = com;
}

Token* appendToTokenSet(TokenSet* tset, Token* t) {
    if (!tset || !t) {
        return NULL;
    }
    Token *insertAfter = NULL;
    if (tset->compare) {
        Token *token = tset->head;
        while (token) {
            if (tset->compare(t, token) == 0)
                return token;
            else if (tset->compare(t, token) < 0) {
                break;
            }
            insertAfter = token;
            token = token->next;
        }
    } else {
        insertAfter = tset->tail;
    }
    tset->tokenCount++;
    if (insertAfter) {
        t->next = insertAfter->next;
        t->prev = insertAfter;
        insertAfter->next = t;
        if (!t->next) {
            tset->tail = t;
            return t;
        }
        t->next->prev = t;
    } else {
        t->next = tset->head;
        tset->head = t;
        if (t->next) {
            t->next->prev = t;
        } else {
            tset->tail = t;
        }
    }
    return t;
}

void printTokenSet(TokenSet* tset) {
    if (!tset) {
        return;
    }
    Token *head = tset->head;
    printf("From HEAD: ");
    while (head) {
        printf("%s->", head->token);
        head = head->next;
    }
    printf("\nFrom TAIL ");
    Token *tail = tset->tail;
    while (tail) {
        printf("%s->", tail->token);
        tail = tail->prev;
    }
    printf("\n");
}

TokenSet_main(int argc, char** argv) {
    printf("TokenSet Test\n");
    char buf[128] = {0};
    TokenSet* tset = newTokenSet();
    setComparator(tset, compareToken);
    while (scanf("%s", buf) != -1) {
        Token *tk = newToken();
        tk->token = malloc(strlen(buf) + 1);
        strcpy(tk->token, buf);
        appendToTokenSet(tset, tk);
        printTokenSet(tset);
    }
}