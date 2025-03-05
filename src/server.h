/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "list.h"
#include "map.h"

#include <event2/event.h>
#include <netinet/in.h>
#include <pthread.h>

struct dchat_server {
    struct event_base * evbase;
    pthread_t evloop;

    struct list port_list;
    pthread_rwlock_t port_list_lock;
};

int dchat_server_init(struct dchat_server *);
int dchat_server_destroy(struct dchat_server *);

int dchat_server_open_port(struct dchat_server *, in_port_t);
int dchat_server_close_port(struct dchat_server *, in_port_t);
