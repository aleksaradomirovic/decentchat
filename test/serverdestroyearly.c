/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test.h"
#include "server.h"

int main() {
    struct dchat_server server;

    if(dchat_server_init(&server) != 0) {
        return -1;
    }

    if(dchat_server_open_port(&server, 20030) != 0) {
        return -1;
    }

    if(dchat_server_destroy(&server) != 0) {
        return -1;
    }

    return 0;
}
