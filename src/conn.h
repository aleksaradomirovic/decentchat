/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "server.h"

#include <netinet/in.h>

struct dchat_conn;

int dchat_conn_init(struct dchat_conn **, struct dchat_server *, const struct sockaddr_in6 *);
int dchat_conn_close(struct dchat_conn *);

int dchat_conn_acquire_packet(struct dchat_conn *, void * pkt, size_t len);

bool dchat_conn_can_recieve(const struct dchat_conn *, const struct sockaddr_in6 *);
