/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _DCHAT_H
#define _DCHAT_H

#define DCHAT_EXPORT __attribute__((visibility ("default")))

#ifdef __cplusplus
extern "C" {
#endif


DCHAT_EXPORT int dchat_init();

DCHAT_EXPORT int dchat_fini();


#ifdef __cplusplus
}
#endif

#endif
