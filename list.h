typedef struct list_node {
	void* value;
	struct list_node *next, *prev;
} list_node;

typedef struct list {
	list_node *head, *tail;
	int count;
	int (*compare)(void* ,void*);
} list;

//Appends the element pointed to by value to list l and returns value
//if value is already present as determined by compare then the original value in the list is returned
void* list_append(list* l, void *value);
void* list_delete(list* l, void *value);
void* list_delete_head(list* l);
int list_count(list* l);
void list_setComparator(list* l, int (*compare)(void* ,void*));
list* newList();

extern list initList;
