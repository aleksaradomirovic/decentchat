/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "server.h"

#include "port.h"

#include <stdlib.h>

static void * dchat_server_event_loop(void * ptr) {
    struct dchat_server * server = (struct dchat_server *) ptr;
    event_base_loop(server->evbase, EVLOOP_NO_EXIT_ON_EMPTY);
    return NULL;
}

static int dchat_server_unref_port(void * ptr) {
    struct dchat_port * port = (struct dchat_port *) ptr;
    if(dchat_port_halt(port) != 0) {
        return -1;
    }

    if(dchat_port_unref(port) != 0) {
        return -1;
    }

    return 0;
}

static int dchat_server_init_port_list(struct dchat_server * server) {
    if(list_init(&server->port_list) != 0) {
        return -1;
    }

    if(pthread_rwlock_init(&server->port_list_lock, NULL) != 0) {
        list_destroy(&server->port_list, NULL);
        return -1;
    }

    return 0;
}

static int dchat_server_destroy_port_list(struct dchat_server * server) {
    if(pthread_rwlock_destroy(&server->port_list_lock) != 0) {
        return -1;
    }

    if(list_destroy(&server->port_list, dchat_server_unref_port) != 0) {
        return -1;
    }

    return 0;
}

static int dchat_server_init_events(struct dchat_server * server) {
    server->evbase = event_base_new();
    if(server->evbase == NULL) {
        return -1;
    }

    if(pthread_create(&server->evloop, NULL, dchat_server_event_loop, server) != 0) {
        event_base_free(server->evbase);
        return -1;
    }

    return 0;
}

static int dchat_server_halt_events(struct dchat_server * server) {
    if(event_base_loopexit(server->evbase, NULL) != 0) {
        return -1;
    }

    if(pthread_join(server->evloop, NULL) != 0) {
        return -1;
    }

    event_base_free(server->evbase);
    return 0;
}

int dchat_server_init(struct dchat_server * server) {
    if(dchat_server_init_port_list(server) != 0) {
        return -1;
    }

    if(dchat_server_init_events(server) != 0) {
        dchat_server_destroy_port_list(server);
        return -1;
    }

    return 0;
}

int dchat_server_destroy(struct dchat_server * server) {
    if(dchat_server_halt_events(server) != 0) {
        return -1;
    }

    if(dchat_server_destroy_port_list(server) != 0) {
        return -1;
    }

    return 0;
}

int dchat_server_open_port(struct dchat_server * server, in_port_t inetport) {
    if(pthread_rwlock_wrlock(&server->port_list_lock) != 0) {
        return -1;
    }
    
    for(struct list_node * node = server->port_list.head; node != NULL; node = node->next) {
        struct dchat_port * port = (struct dchat_port *) node->element;
        if(port->addr.sin6_port == inetport) {
            pthread_rwlock_unlock(&server->port_list_lock);
            return -1;
        }
    }

    struct dchat_port * port = malloc(sizeof(struct dchat_port));
    if(port == NULL) {
        pthread_rwlock_unlock(&server->port_list_lock);
        return -1;
    }

    if(dchat_port_init(port, inetport, server) != 0) {
        free(port);
        pthread_rwlock_unlock(&server->port_list_lock);
        return -1;
    }

    if(list_add_tail(&server->port_list, port) != 0) {
        free(port);
        pthread_rwlock_unlock(&server->port_list_lock);
        return -1;
    }

    if(pthread_rwlock_unlock(&server->port_list_lock) != 0) {
        list_remove_tail(&server->port_list, dchat_server_unref_port);
        return -1;
    }

    return 0;
}

int dchat_server_close_port(struct dchat_server * server, in_port_t inetport) {
    if(pthread_rwlock_wrlock(&server->port_list_lock) != 0) {
        return -1;
    }

    for(struct list_node * node = server->port_list.head; node != NULL; node = node->next) {
        struct dchat_port * port = (struct dchat_port *) node->element;
        if(port->addr.sin6_port == inetport) {
            if(list_remove_node(&server->port_list, node, dchat_server_unref_port) != 0) {
                pthread_rwlock_unlock(&server->port_list_lock);
                return -1;
            }

            if(pthread_rwlock_unlock(&server->port_list_lock) != 0) {
                return -1;
            }

            return 0;
        }
    }
    
    pthread_rwlock_unlock(&server->port_list_lock);
    return -1;
}
