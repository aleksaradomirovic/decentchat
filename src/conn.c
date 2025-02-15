/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "conn.h"

#include <stdlib.h>
#include <string.h>

struct dchat_conn {
    struct dchat_server * server;

    struct sockaddr_in6 addr;
};

int dchat_conn_init(struct dchat_conn ** ptr, struct dchat_server * server, const struct sockaddr_in6 * addr) {
    struct dchat_conn * conn = malloc(sizeof(struct dchat_conn));
    if(conn == NULL) {
        return -1;
    }

    conn->addr = *addr;
    conn->server = server;

    *ptr = conn;
    return 0;
}

int dchat_conn_close(struct dchat_conn * conn) {
    free(conn);
    return 0;
}

bool dchat_conn_can_recieve(const struct dchat_conn * conn, const struct sockaddr_in6 * addr) {
    return (addr->sin6_family == AF_INET6)
        && (addr->sin6_port == conn->addr.sin6_port)
        && (memcmp(&addr->sin6_addr, &conn->addr.sin6_addr, sizeof(addr->sin6_addr)) == 0)
        // && (addr->sin6_flowinfo == /* conn->addr.sin6_flowinfo */ 0)
        // && (addr->sin6_scope_id == /* conn->addr.sin6_scope_id */ 0)
        ;
}

int dchat_conn_acquire_packet(struct dchat_conn * conn, void * pkt, size_t len) {
    return 0; // TODO
}
