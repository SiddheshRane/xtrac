typedef struct listNode {
    void* value;
    struct listNode *next, *prev;
} listNode;

typedef struct list {
    listNode *head, *tail;
    int count;
    int (*compare)(void*, void*);
    int distinctElements;
} list;

typedef struct iterator {
    listNode *current;
    list* l;
    int index;
    int remaining;
} iterator;


/* Appends the element pointed to by value to list l and returns value.
 * If value is already present as determined by compare then the original value in the list is returned
 */
void* listAppend(list* l, void *value);
void* listDelete(list* l, void *value);
void* listDeleteHead(list* l);
int listCount(list* l);
void listSetComparator(list* l, int (*compare)(void*, void*));
list* newList();

iterator* listGetIterator(list* l);
void* listGetNext(iterator* it);
void* listDeleteNext(iterator* it);
extern const list initList;
