/*
 * Copyright (C) 2025 Aleksa Radomirovic 
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "conn.h"
#include "server.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct dchat_conn_list_node {
    struct dchat_conn * conn;
    struct dchat_conn_list_node * next;
};

struct dchat_server {
    int sockfd;

    struct dchat_conn_list_node * connections;

    struct sockaddr_in6 addr;
};

static int dchat_server_port_init(const struct sockaddr_in6 * addr) {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        return -1;
    }

    if(fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        close(sockfd);
        return -1;
    }

    if(bind(sockfd, (const struct sockaddr *) addr, sizeof(*addr)) != 0) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int dchat_server_init(struct dchat_server ** ptr, in_port_t port) {
    struct dchat_server * server = malloc(sizeof(struct dchat_server));
    if(server == NULL) {
        return -1;
    }

    server->addr = (struct sockaddr_in6) {
        .sin6_family = AF_INET6,
        .sin6_port = port,
        .sin6_flowinfo = 0,
        .sin6_addr = in6addr_any,
        .sin6_scope_id = 0,
    };

    if((server->sockfd = dchat_server_port_init(&server->addr)) == -1) {
        free(server);
        return -1;
    }

    server->connections = NULL;
    *ptr = server;
    return 0;
}

int dchat_server_close(struct dchat_server * server) {
    for(struct dchat_conn_list_node * node = server->connections; node != NULL;) {
        struct dchat_conn_list_node * next = node->next;

        dchat_conn_close(node->conn);
        free(node);

        node = next;
    }

    shutdown(server->sockfd, SHUT_RDWR);
    close(server->sockfd);
    free(server);
    return 0;
}

in_port_t dchat_server_get_port(const struct dchat_server * server) {
    return server->addr.sin6_port;
}

int dchat_server_poll(struct dchat_server * server) {
    char dummy[0];
    ssize_t pkt_size;
    if((pkt_size = recvfrom(server->sockfd, dummy, 0, MSG_TRUNC, NULL, NULL)) < 0) {
        return -1;
    }

    void * pkt = malloc(pkt_size);
    if(pkt == NULL) {
        return -1;
    }

    struct sockaddr_in6 addr;
    socklen_t addrlen = sizeof(addr);

    if(recvfrom(server->sockfd, pkt, pkt_size, 0, (struct sockaddr *) &addr, &addrlen) != pkt_size) {
        free(pkt);
        return -1;
    }


    for(struct dchat_conn_list_node * node = server->connections; node != NULL; node = node->next) {
        if(dchat_conn_can_recieve(node->conn, &addr)) {
            if(dchat_conn_acquire_packet(node->conn, pkt, pkt_size) == 0) {
                return 0;
            }
        }
    }

    return -1;
}
