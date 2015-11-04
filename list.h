typedef struct list_node {
    void* value;
    struct list_node *next, *prev;
} list_node;

typedef struct list {
    list_node *head, *tail;
    int count;
    int (*compare)(void*, void*);
int distinctElements;
} list;

//Appends the element pointed to by value to list l and returns value
//if value is already present as determined by compare then the original value in the list is returned
void* listAppend(list* l, void *value);
void* listDelete(list* l, void *value);
void* listDeleteHead(list* l);
int listCount(list* l);
void listSetComparator(list* l, int (*compare)(void*, void*));
list* newList();

extern list initList;
