/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "dchat/dchat.h"

#include <event2/thread.h>

int dchat_init() {
    if(evthread_use_pthreads() != 0) {
        return -1;
    }

    return 0;
}

int dchat_fini() {
    return 0;
}
