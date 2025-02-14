/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <netinet/in.h>

struct dchat_server;

int dchat_server_init(struct dchat_server **, in_port_t);
int dchat_server_incref(struct dchat_server *);
int dchat_server_decref(struct dchat_server *);

in_port_t dchat_server_get_port(struct dchat_server *);
