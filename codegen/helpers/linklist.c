#include "linklist.h"

void list_init(list_t *list) {
    list->head = NULL;
    list->list_size = 0;
}

bool list_is_empty(list_t *list) {
    return list->head == NULL;
}

void list_insert_head(list_t *list, void *data) {
    list_node_t *new_node = (list_node_t *)malloc(sizeof(list_node_t));
    if (!new_node) {
        printf("Memory allocation failed in list_insert_head\n");
        return;
    }
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->list_size++;
}

void list_insert_tail(list_t *list, void *data) {
    list_node_t *new_node = (list_node_t *)malloc(sizeof(list_node_t));
    if (!new_node) {
        printf("Memory allocation failed in list_insert_tail\n");
        return;
    }
    new_node->data = data;
    new_node->next = NULL;

    if (!list->head) list->head = new_node;

    else {
        list_node_t *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->list_size++;
}

void *list_remove_head(list_t *list) {
    if (!list->head) {
        printf("List is empty!\n");
        return NULL;
    }
    list_node_t *temp = list->head;
    void *data = temp->data;
    list->head = temp->next;
    list->list_size--;
    free(temp);
    return data;
}

void list_destroy(list_t *list) {
    list_node_t *current = list->head;
    while (current) {
        list_node_t *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
}

