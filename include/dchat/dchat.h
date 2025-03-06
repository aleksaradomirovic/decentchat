/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _DCHAT_H
#define _DCHAT_H

#include <stdint.h>

#define DCHAT_EXPORT __attribute__((visibility ("default")))

#ifdef __cplusplus
extern "C" {
#endif


DCHAT_EXPORT int dchat_init();

DCHAT_EXPORT int dchat_fini();

typedef void * dchat_t;

DCHAT_EXPORT int dchat_open(dchat_t *);

DCHAT_EXPORT int dchat_close(dchat_t);

typedef uint16_t dchat_port_t;

DCHAT_EXPORT int dchat_open_port(dchat_t, dchat_port_t);

DCHAT_EXPORT int dchat_close_port(dchat_t, dchat_port_t);


#ifdef __cplusplus
}
#endif

#endif
