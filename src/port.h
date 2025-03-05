/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdatomic.h>
#include <event2/event.h>
#include <netinet/in.h>

struct dchat_server;

struct dchat_port {
    atomic_size_t refs;

    int sockfd;
    struct event * ev;
    struct dchat_server * server;

    struct sockaddr_in6 addr;
};

int dchat_port_init(struct dchat_port *, in_port_t, struct dchat_server *);
int dchat_port_destroy(struct dchat_port *);
