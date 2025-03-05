/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "server.h"

#include <event2/thread.h>
#include <stdlib.h>

__attribute__((constructor))
void setup_libevent() {
    if(evthread_use_pthreads() != 0) {
        exit(-1);
    }
}

static void * dchat_server_event_loop(void * ptr) {
    struct dchat_server * server = (struct dchat_server *) ptr;
    event_base_loop(server->evbase, EVLOOP_NO_EXIT_ON_EMPTY);
    return NULL;
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

int dchat_server_init(struct dchat_server * server) {
    if(dchat_server_init_events(server) != 0) {
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

int dchat_server_destroy(struct dchat_server * server) {
    if(dchat_server_halt_events(server) != 0) {
        return -1;
    }

    return 0;
}
