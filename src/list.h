/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdlib.h>

struct list_node {
    struct list_node * prev, * next;
    void * element;
};

static struct list_node * list_node_create(void * element) {
    struct list_node * node = malloc(sizeof(struct list_node));
    if(node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->prev = NULL;

    node->element = element;
    return node;
}

struct list {
    struct list_node * head, * tail;
};

[[maybe_unused]]
static int list_init(struct list * list) {
    list->head = NULL;
    list->tail = NULL;

    return 0;
}

[[maybe_unused]]
static int list_remove_node(struct list * list, struct list_node * node, int (*element_free)(void *)) {
    if(element_free != NULL) {
        if(element_free(node->element) != 0) {
            return -1;
        }
    }

    if(node->prev == NULL) {
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }

    if(node->next == NULL) {
        list->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    free(node);

    return 0;
}

[[maybe_unused]]
static int list_add_tail(struct list * list, void * element) {
    struct list_node * node = list_node_create(element);
    if(node == NULL) {
        return -1;
    }

    node->prev = list->tail;
    node->next = NULL;

    list->tail = node;
    if(list->head == NULL) list->head = node;
    return 0;
}

[[maybe_unused]]
static int list_remove_tail(struct list * list, int (*element_free)(void *)) {
    if(list->tail == NULL) {
        return -1;
    }

    if(list_remove_node(list, list->tail, element_free) != 0) {
        return -1;
    }

    return 0;
}

[[maybe_unused]]
static int list_insert_before(struct list * list, void * element, struct list_node * next) {
    struct list_node * node = list_node_create(element);
    if(node == NULL) {
        return -1;
    }

    node->prev = next->prev;
    node->next = next;

    if(next->prev == NULL) {
        list->head = node;
    } else {
        next->prev->next = node;
    }
    next->prev = node;
}

[[maybe_unused]]
static int list_clear(struct list * list, int (*element_free)(void *)) {
    for(struct list_node * node = list->head; node != NULL;) {
        struct list_node * next = node->next;
        if(list_remove_node(list, node, element_free) != 0) {
            return -1;
        }
        node = next;
    }

    return 0;
}

[[maybe_unused]]
static int list_destroy(struct list * list, int (*element_free)(void *)) {
    return list_clear(list, element_free);
}
