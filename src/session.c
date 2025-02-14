/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "session.h"

#include <pthread.h>
#include <stdlib.h>

struct dchat_server_list_node {
    struct dchat_server * server;
    struct dchat_server_list_node * next;
};

struct dchat_session {
    struct dchat_server_list_node * servers_list;
    pthread_mutex_t servers_list_mutex;

    pthread_t poll_thread;
};

static void * poll_thread_routine(void * arg) {
    struct dchat_session * session = arg;
    return NULL;
}

int dchat_session_init(struct dchat_session ** ptr) {
    struct dchat_session * session = malloc(sizeof(struct dchat_session));
    if(session == NULL) {
        return -1;
    }

    if(pthread_mutex_init(&session->servers_list_mutex, NULL) != 0) {
        free(session);
        return -1;
    }

    session->servers_list = NULL;

    if(pthread_create(&session->poll_thread, NULL, poll_thread_routine, session) != 0) {
        pthread_mutex_destroy(&session->servers_list_mutex);
        free(session);
        return -1;
    }

    *ptr = session;
    return 0;
}

int dchat_session_close(struct dchat_session * session) {
    if(pthread_mutex_lock(&session->servers_list_mutex) != 0) {
        return -1;
    }

    for(struct dchat_server_list_node * node = session->servers_list; node != NULL;) {
        dchat_server_decref(node->server);
        struct dchat_server_list_node * next = node->next;
        free(node);
        node = next;
    }
    session->servers_list = NULL;

    pthread_mutex_unlock(&session->servers_list_mutex);
    pthread_join(session->poll_thread, NULL);
    pthread_mutex_unlock(&session->servers_list_mutex);

    pthread_mutex_destroy(&session->servers_list_mutex);
    free(session);
    return 0;
}

int dchat_session_open_server(struct dchat_session * session, in_port_t port) {
    if(pthread_mutex_lock(&session->servers_list_mutex) != 0) {
        return -1;
    }

    struct dchat_server_list_node ** nodeptr = &session->servers_list;
    for(; *nodeptr != NULL; nodeptr = &(*nodeptr)->next) {
        if(dchat_server_get_port((*nodeptr)->server) == port) {
            pthread_mutex_unlock(&session->servers_list_mutex);
            return -1;
        }
    }

    struct dchat_server_list_node * new_node = malloc(sizeof(struct dchat_server_list_node));
    if(new_node == NULL) {
        pthread_mutex_unlock(&session->servers_list_mutex);
        return -1;
    }
    new_node->next = NULL;

    if(dchat_server_init(&new_node->server, port) != 0) {
        free(new_node);
        pthread_mutex_unlock(&session->servers_list_mutex);
        return -1;
    }

    pthread_mutex_unlock(&session->servers_list_mutex);
    *nodeptr = new_node;
    return 0;
}

int dchat_session_close_server(struct dchat_session * session, in_port_t port) {
    if(pthread_mutex_lock(&session->servers_list_mutex) != 0) {
        return -1;
    }

    struct dchat_server_list_node ** nodeptr = &session->servers_list;
    for(;; nodeptr = &(*nodeptr)->next) {
        if(*nodeptr == NULL) {
            pthread_mutex_unlock(&session->servers_list_mutex);
            return -1;
        }

        if(dchat_server_get_port((*nodeptr)->server) == port) {
            break;
        }
    }

    struct dchat_server_list_node * node = *nodeptr;
    *nodeptr = node->next;

    dchat_server_decref(node->server);
    free(node);

    pthread_mutex_unlock(&session->servers_list_mutex);
    return 0;
}
