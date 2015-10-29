#include  <stdlib.h>
#include "list.h"

void* list_append(list* l, void *t) {
    if (!l || !t) {
        return NULL;
    }
    list_node *insertAfter = NULL;
    if (l->compare) {
        list_node *node = l->head;
        while (node) {
            if (l->compare(t, node->value) == 0)
                return node->value;
            else if (l->compare(t, node->value) < 0) {
                break;
            }
            insertAfter = node;
            node = node->next;
        }
    } else {
        insertAfter = l->tail;
    }
	list_node* newNode = malloc(sizeof(list_node));
	if(!newNode)
		return NULL;
    l->count++;
	newNode->value = t;
    if (insertAfter) {
        newNode->next = insertAfter->next;
        newNode->prev = insertAfter;
        insertAfter->next = newNode;
        if (!newNode->next) {
            l->tail = newNode;
            return t;
        }
        newNode->next->prev = newNode;
    } else {
        newNode->next = l->head;
        l->head = newNode;
        if (newNode->next) {
            newNode->next->prev = newNode;
        } else {
            l->tail = newNode;
        }
    }
    return t;
}

int list_count(list* l) {
	if(!l)
		return 0;
	return l->count;
}

void list_setComparator(list* l, int (*compare)(void* ,void*)) {
	if(!l)
		return;
	l->compare = compare;
}
