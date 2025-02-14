/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "server.h"

struct dchat_session;

int dchat_session_init(struct dchat_session **);
int dchat_session_close(struct dchat_session *);

int dchat_session_open_server(struct dchat_session *, in_port_t);
int dchat_session_close_server(struct dchat_session *, in_port_t);
