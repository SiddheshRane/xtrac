#include  <stdlib.h>
#include <stdio.h>
#include "list.h"

list initList = {0, 0, 0, 0};

void* list_append(list* l, void *t) {
    if (!l || !t) {
        return NULL;
    }
    list_node *insertAfter = NULL;
    if (l->compare) {
        list_node *node = l->head;
        while (node) {
            /* if (l->compare(t, node->value) == 0)
                 return node->value;
             else */
            if (l->compare(t, node->value) < 0) {
                break;
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

void* list_delete(list *l, void* value) {
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

void* list_delete_head(list* l) {
    if (!l || !l->head)
        return NULL;
    return list_delete(l, l->head->value);
}

int list_count(list* l) {
    if (!l)
        return 0;
    return l->count;
}

void list_setComparator(list* l, int (*compare)(void*, void*)) {
    if (!l)
        return;
    l->compare = compare;
}

void list_print(list *l, void (*print)(void*)) {
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
