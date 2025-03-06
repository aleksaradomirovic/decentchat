/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "dchat/dchat.h"

#include "server.h"

#include <event2/thread.h>

int dchat_init() {
    if(evthread_use_pthreads() != 0) {
        return -1;
    }

    return 0;
}

int dchat_fini() {
    return 0;
}

int dchat_open(dchat_t * ptr) {
    struct dchat_server * server = malloc(sizeof(struct dchat_server));
    if(server == NULL) {
        return -1;
    }

    if(dchat_server_init(server) != 0) {
        free(server);
        return -1;
    }

    *ptr = server;
    return 0;
}

int dchat_close(dchat_t ptr) {
    struct dchat_server * server = (struct dchat_server *) ptr;
    
    if(dchat_server_destroy(server) != 0) {
        return -1;
    }

    free(server);
    return 0;
}

int dchat_open_port(dchat_t ptr, dchat_port_t inetport) {
    struct dchat_server * server = (struct dchat_server *) ptr;

    if(dchat_server_open_port(server, inetport) != 0) {
        return -1;
    }

    return 0;
}

int dchat_close_port(dchat_t ptr, dchat_port_t inetport) {
    struct dchat_server * server = (struct dchat_server *) ptr;

    if(dchat_server_close_port(server, inetport) != 0) {
        return -1;
    }

    return 0;
}
