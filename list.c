#include  <stdlib.h>
#include <stdio.h>
#include "list.h"

list initList = {0, 0, 0, 0, 0};

list* newList() {
    list *l = calloc(1, sizeof (list));
    return l;
}

void* listAppend(list* l, void *t) {
    if (!l || !t) {
        return NULL;
    }
    list_node *insertAfter = NULL;
    if (l->compare) {
        list_node *node = l->head;
        while (node) {
            if (l->compare(t, node->value) < 0) {
                break;
            } else if (l->distinctElements && l->compare(t, node->value) == 0) {
                return node->value;
            }
            insertAfter = node;
            node = node->next;
        }
    } else {
        insertAfter = l->tail;
    }
    list_node* newNode = malloc(sizeof (list_node));
    if (!newNode)
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

void* listDelete(list *l, void* value) {
    if (!l || !value) {
        return NULL;
    }
    list_node* node = l->head;
    while (node) {
        if (node->value == value)
            break;
        node = node->next;
    }
    if (!node)
        return NULL;
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        l->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        l->tail = node->prev;
    }
    free(node);
    l->count--;
    return value;
}

void* listDeleteHead(list* l) {
    if (!l || !l->head)
        return NULL;
    return listDelete(l, l->head->value);
}

int listCount(list* l) {
    if (!l)
        return 0;
    return l->count;
}

void listSetComparator(list* l, int (*compare)(void*, void*)) {
    if (!l)
        return;
    l->compare = compare;
}

void listPrint(list *l, void (*print)(void*)) {
    if (!l) {
        printf("Empty!\n");
        return;
    }
    list_node* node = l->head;
    printf("List contains %d elements:", l->count);
    while (node) {
        print(node->value);
        node = node->next;
    }
}

void listSetDistinctElements(list l, int bool) {
    if (!l) {
        return;
    }
    l->distinctElements = bool;

}
