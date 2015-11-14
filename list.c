/* 
 * Copyright (C) 2015 Siddhesh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include  <stdlib.h>
#include <stdio.h>
#include "list.h"

const list initList = {0, 0, 0, 0, 0};
static listNode* implGetNodeFrom(listNode* from, void* value);
static void* implDeleteNode(list* l, listNode* node);

list* newList() {
    list *l = calloc(1, sizeof (list));
    return l;
}

void* listAppend(list* l, void *t) {
    if (!l || !t) {
        return NULL;
    }
    listNode *insertAfter = NULL;
    if (l->compare) {
        listNode *node = l->head;
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
    listNode* newNode = malloc(sizeof (listNode));
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

static listNode* implGetNodeFrom(listNode* from, void* value) {
    listNode* node = from;
    while (node) {
        if (node->value == value)
            return node;
        node = node->next;
    }
    return NULL;
}

static void* implDeleteNode(list* l, listNode* node) {
    if (!node)
        return NULL;
    void* value = node->value;
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

void* listDelete(list *l, void* value) {
    if (!l || !value) {
        return NULL;
    }
    listNode* node = implGetNodeFrom(l->head, value);
    return implDeleteNode(l, node);
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
    listNode* node = l->head;
    printf("List contains %d elements:", l->count);
    while (node) {
        print(node->value);
        node = node->next;
    }
}

void listSetDistinctElements(list* l, int bool) {
    if (!l) {
        return;
    }
    l->distinctElements = bool;

}

iterator* listGetIterator(list* l) {
    if (!l) {
        return NULL;
    }
    iterator *it = malloc(sizeof (iterator));
    if (!it) {
        return NULL;
    }
    it->current = l->head;
    it->index = 0;
    it->l = l;
    it->remaining = l->count;
    return it;
}

void* listGetNext(iterator* it) {
    if (!it || !it->current) {
        return NULL;
    }
    void* value = it->current->value;
    it->current = it->current->next;
    it->index++;
    it->remaining--;
    return value;
}

void* listDeleteNext(iterator* it) {
    if (!it || !it->current) {
        return NULL;
    }
    listNode* toDelete = it->current;
    it->current = toDelete->next;
    it->remaining--;
    return implDeleteNode(it->l, it->current);
}