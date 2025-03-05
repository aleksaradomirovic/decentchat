/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "list.h"

typedef list_free_t map_free_t;
typedef int (* map_compare_t)(const void * lhs, const void * rhs);

struct map_bucket {
    struct list list;
};

static int map_bucket_init(struct map_bucket * bucket) {
    if(list_init(&bucket->list) != 0) {
        return -1;
    }

    return 0;
}

static int map_bucket_destroy(struct map_bucket * bucket, map_free_t element_free) {
    if(list_destroy(&bucket->list, element_free) != 0) {
        return -1;
    }

    return 0;
}

struct map {
    struct map_bucket * buckets;
    size_t bucket_count;
};

[[maybe_unused]]
static int map_init(struct map * map, size_t bucket_count) {
    map->bucket_count = bucket_count;
    map->buckets = calloc(bucket_count, sizeof(struct map_bucket));
    if(map->buckets == NULL) {
        return -1;
    }

    for(size_t i = 0; i < bucket_count; i++) {
        if(map_bucket_init(&map->buckets[i]) != 0) {
            free(map->buckets);
            return -1;
        }
    }

    return 0;
}

static struct list * map_get_bucket_list(struct map * map, size_t hash) {
    return &map->buckets[hash % (map->bucket_count)].list;
}

static struct list_node * map_bucket_list_find_first_not_less(struct list * list, const void * element, map_compare_t element_compare, int * cmp) {
    *cmp = -1;

    for(struct list_node * node = list->head; node != NULL; node = node->next) {
        *cmp = element_compare(element, node->element);
        if(*cmp < 0) {
            continue;
        } else if(*cmp > 0) {
            return NULL;
        } else {
            return node;
        }
    }

    return NULL;
}

static struct list_node * map_bucket_list_find(struct list * list, const void * element, map_compare_t element_compare) {
    int cmp;
    struct list_node * node = map_bucket_list_find_first_not_less(list, element, element_compare, &cmp);
    if(node == NULL || cmp != 0) {
        return NULL;
    }

    return node;
}

[[maybe_unused]]
static int map_insert(struct map * map, void * element, size_t hash, map_compare_t element_compare) {
    struct list * bucket_list = map_get_bucket_list(map, hash);

    int cmp;
    struct list_node * next = map_bucket_list_find_first_not_less(bucket_list, element, element_compare, &cmp);
    if(cmp == 0) {
        return -1;
    }

    if(list_insert_before(bucket_list, element, next) != 0) {
        return -1;
    }

    return 0;
}

[[maybe_unused]]
static struct list_node * map_find(struct map * map, const void * element, size_t hash, map_compare_t element_compare) {
    struct list * bucket_list = map_get_bucket_list(map, hash);
    return map_bucket_list_find(bucket_list, element, element_compare);
}

[[maybe_unused]]
static int map_remove(struct map * map, const void * element, size_t hash, map_compare_t element_compare, map_free_t element_free) {
    struct list * bucket_list = map_get_bucket_list(map, hash);
    struct list_node * node = map_bucket_list_find(bucket_list, element, element_compare);
    if(node == NULL) {
        return -1;
    }

    if(list_remove_node(bucket_list, node, element_free) != 0) {
        return -1;
    }

    return 0;
}

[[maybe_unused]]
static int map_destroy(struct map * map, map_free_t element_free) {
    for(size_t i = 0; i < map->bucket_count; i++) {
        if(map_bucket_destroy(&map->buckets[i], element_free) != 0) {
            return -1;
        }
    }

    free(map->buckets);
    return 0;
}
