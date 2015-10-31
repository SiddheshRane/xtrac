
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Prefix {
    int freq;
    char ascii;
    struct Prefix *zero, *one;
} Prefix;

int levelWiseCallback(Prefix*,int);
static void traverse(Prefix*, int (*compare)(Prefix*,int));
void flattenTree(Prefix*);
void printTree(Prefix**, int);

typedef struct HuffmanTree {
    Prefix *root;
    int maxHeight;
} HuffmanTree;

Prefix* newPrefix() {
	Prefix *p = calloc(1, sizeof(Prefix));
	return p;
}

int comparePrefix(void* i1, void* i2) {
    int a = ((Prefix*)i1)->freq;
    int b = ((Prefix*)i2)->freq;
    if (a == b)
        return 0;
    if (a > b) {
        return 1;
    }
    return -1;
}

HuffmanTree* buildHuffmanTree(int count[256]) {
    list l = initList;
    list_setComparator(&l, comparePrefix);
    int i;
    for (i = 0; i < 256; i++) {
        Prefix *p = newPrefix();
        if(!p)
        	return NULL;
        p->ascii = i;
        p->freq = count[i];
        list_append(&l, p);
    }
    
    Prefix *p1, *p2;
    while((p1 = list_delete_head(&l)) && (p2 = list_delete_head(&l))) {
    	Prefix *newP = newPrefix();
    	newP->freq = p1->freq + p2->freq;
    	newP->zero = p1;
    	newP->one = p2;
    	list_append(&l, newP);   	
    }
    HuffmanTree *hf = calloc(1, sizeof(HuffmanTree));
    p1 = p1 == NULL ? p2 : p1;
    hf->root = p1;
    flattenTree(hf->root);
    return hf;
}



/////////////STACK RELATED///////////////
typedef struct treelevel {
	Prefix *t;
	int level;
} treelevel;
typedef struct stack {
	struct treelevel stk[4096];
	int top;
} stack;
typedef struct stack* treestack;

void initstack(treestack *ts);
void push(treestack ts, treelevel n);
treelevel pop(treestack ts);

void initstack(treestack *ts) {
	*ts = malloc(sizeof(struct stack));
	if(!*ts)
		return;
	(*ts)->top = -1;
}
void push(treestack ts, treelevel tl) {
	if(!tl.t)
		return;
	ts->top++;
	ts->stk[ts->top] = tl;
}
const treelevel empty = {NULL, -1};
treelevel pop(treestack ts) {
	if (ts->top < 0)
	{
		return empty;
	}
	treelevel ret = ts->stk[ts->top];
	ts->top--;
	return ret;
}

/* 
 * a generic function which traverses the tree in preorder.
 */
static void traverse(Prefix *t, int (*callback)(Prefix*, int)) {
	if (!t)
	{
		return;
	}
	treestack ts = NULL;
	treelevel tl;
	initstack(&ts);
	int level = 0;
	int rightLevelOffset = 0;
	/* 1. If you have a left, go there. Repeat 1.
	 * 2. If you have a right, go there. Repeat from 1.
	 * 3. If you have a leaf node, go where the top of the stack points
	 */
	do {
		
		if(callback(t,level))
			break;
		if(t->zero) {
			tl.t = t->one;
			tl.level = level+1;
			push(ts, tl);
			t = t->zero;
			level++;
		}
		else if(t->one) {
			t = t->one;
			level++;
		}
		//leaf node. go where the stack top points
		else {
			tl = pop(ts);
			t = tl.t;
			level = tl.level;
		}
	} while (t);
}

/*
 * Level Wise Tree array Callback
 */
Prefix valid = {0};
Prefix* lineTree[8192];
int lwtSize = 0;
int maxLevel = 0;


int levelWiseCallback(Prefix *t, int level) {
	maxLevel = level > maxLevel ? level : maxLevel;
	int index = (1 << level) - 1;
	while(lineTree[index] != &valid)
		index++;
		
	lineTree[index] = t;
	lwtSize++;
	
	if(t->zero) {
		lineTree[index*2 + 1] = &valid;
	}
	if(t->one) {
		lineTree[(index+1)*2] = &valid;
	}
	return 0;
}

void flattenTree(Prefix* t) {
	lwtSize = 0;
	maxLevel = 0;
	memset(lineTree, 0, sizeof(Prefix*)*8192);
	lineTree[0] = &valid;
	traverse(t, levelWiseCallback);
	printTree(lineTree, maxLevel);
}

/////////Tree Printing//////////
void printNtimes(char c, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%c", c);
	}
}
void printSpace(int num) {
	printNtimes(' ', num);
}
void printUnderScore(int num) {
	printNtimes('_', num);
}

void printTree(Prefix **t, int level){
	if(!t || !*t)
		return;
	int i, j;
	char ch;
	int numSpaces;
	for (i = level; i > -1; i -= 1) {
		numSpaces = (1 << i);
		numSpaces  /= 2;
		for(j = 0; j < 1 << (level-i); j++) {
			ch = ' ';
			printNtimes(' ', numSpaces);
			if(*t && (*t)->zero)
				ch = '_';
			printNtimes(ch, numSpaces-1);
			if(*t)
				ch = (*t)->ascii;
			else
				ch = ' ';
			printf("%c",ch);
			if(*t && (*t)->one)
				ch = '_';
			else
				ch = ' ';
			printNtimes(ch, numSpaces-1);
			printNtimes(' ',numSpaces);
			printf(" ");
			t++;
		}
		printf("\n");
		if(!i)
			break;
		t = t - j ;
		for(j = 0; j < 1 << (level-i); j++) {
			printNtimes(' ', numSpaces-1);
			if(*t && (*t)->zero)
				ch = '/';
			else 
				ch = ' ';
			printf("%c", ch);
			printNtimes(' ', (numSpaces-1)*2 + 1);
			if(*t && (*t)->one)
				ch = '\\';
			else 
				ch = ' ';
			printf("%c", ch);
			printNtimes(' ', numSpaces);
			t++;
		}
		printf("\n");
	}
}
