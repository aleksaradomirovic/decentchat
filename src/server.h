/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "port.h"

#include <event2/event.h>
#include <pthread.h>

struct dchat_server {
    struct event_base * evbase;
    pthread_t evloop;
};

int dchat_server_init(struct dchat_server *);
int dchat_server_destroy(struct dchat_server *);
