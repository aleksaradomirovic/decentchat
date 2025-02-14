/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "server.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct dchat_server {
    int sockfd;
    size_t refcount;

    struct sockaddr_in6 addr;
    pthread_mutex_t mutex;
};

static int dchat_server_port_init(const struct sockaddr_in6 * addr) {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        return -1;
    }

    if(bind(sockfd, (const struct sockaddr *) addr, sizeof(*addr)) != 0) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int dchat_server_init(struct dchat_server ** ptr, in_port_t port) {
    struct dchat_server * server = malloc(sizeof(struct dchat_server));
    if(server == NULL) {
        return -1;
    }

    if(pthread_mutex_init(&server->mutex, NULL) != 0) {
        free(server);
        return -1;
    }

    server->addr = (struct sockaddr_in6) {
        .sin6_family = AF_INET6,
        .sin6_port = port,
        .sin6_flowinfo = 0,
        .sin6_addr = in6addr_any,
        .sin6_scope_id = 0,
    };

    if((server->sockfd = dchat_server_port_init(&server->addr)) == -1) {
        pthread_mutex_destroy(&server->mutex);
        free(server);
        return -1;
    }

    atomic_store_explicit(&server->refcount, 1, memory_order_release);
    *ptr = server;
    return 0;
}

int dchat_server_incref(struct dchat_server * server) {
    atomic_fetch_add_explicit(&server->refcount, 1, memory_order_acquire);
    return 0;
}

static int dchat_server_close(struct dchat_server * server) {
    pthread_mutex_destroy(&server->mutex);
    shutdown(server->sockfd, SHUT_RDWR);
    close(server->sockfd);
    free(server);
    return 0;
}

int dchat_server_decref(struct dchat_server * server) {
    if(atomic_fetch_sub_explicit(&server->refcount, 1, memory_order_release) == 1) {
        dchat_server_close(server);
    }
    return 0;
}

in_port_t dchat_server_get_port(struct dchat_server * server) {
    return server->addr.sin6_port;
}
