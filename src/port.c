/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "port.h"

#include "server.h"

#include <sys/socket.h>
#include <unistd.h>

static void dchat_port_socket_event_cb(int sockfd, short event, void * ptr) {

}

static int dchat_port_init_socket_and_event(struct dchat_port * port, struct dchat_server * server) {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        return -1;
    }

    if(evutil_make_socket_nonblocking(sockfd) != 0) {
        close(sockfd);
        return -1;
    }

    struct event * ev = event_new(server->evbase, sockfd, EV_READ | EV_PERSIST, dchat_port_socket_event_cb, port);
    if(ev == NULL) {
        close(sockfd);
        return -1;
    }

    port->sockfd = sockfd;
    port->ev = ev;

    return 0;
}

static int dchat_port_destroy_socket_and_event(struct dchat_port * port) {
    event_free(port->ev);
    shutdown(port->sockfd, SHUT_RDWR);
    if(close(port->sockfd) != 0) {
        return -1;
    }

    return 0;
}

static int dchat_port_finalize(struct dchat_port * port, in_port_t inetport) {
    port->addr = (struct sockaddr_in6) {
        AF_INET6,
        inetport,
        0,
        in6addr_any,
        0
    };

    if(event_add(port->ev, NULL) != 0) {
        return -1;
    }

    if(bind(port->sockfd, (const struct sockaddr *) &port->addr, sizeof(struct sockaddr_in6)) != 0) {
        event_del(port->ev);
        return -1;
    }

    return 0;
}

static int dchat_port_halt_event(struct dchat_port * port) {
    if(event_del(port->ev) != 0) {
        return -1;
    }

    return 0;
}

int dchat_port_init(struct dchat_port * port, in_port_t inetport, struct dchat_server * server) {
    if(dchat_port_init_socket_and_event(port, server) != 0) {
        return -1;
    }

    if(dchat_port_finalize(port, inetport) != 0) {
        dchat_port_destroy_socket_and_event(port);
        return -1;
    }
    
    return 0;
}

int dchat_port_destroy(struct dchat_port * port) {
    if(dchat_port_halt_event(port) != 0) {
        return -1;
    }

    if(dchat_port_destroy_socket_and_event(port) != 0) {
        return -1;
    }

    return 0;
}
