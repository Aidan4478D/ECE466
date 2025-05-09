#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    void *data;
    struct node *next;
} list_node_t;

typedef struct {
    list_node_t *head;
} list_t;

void list_init(list_t *list);
bool list_is_empty(list_t *list);
void list_insert_head(list_t *list, void *data);
void list_insert_tail(list_t *list, void *data);
void* list_remove_head(list_t *list);
void list_destroy(list_t *list);

#endif

