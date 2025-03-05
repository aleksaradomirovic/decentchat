/*
 * Copyright (C) 2025 Aleksa Radomirovic
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "dchat/dchat.h"

#include <event2/event.h>
#include <stdlib.h>

__attribute__((constructor))
static void test_init() {
    if(dchat_init() != 0) {
        exit(-1);
    }
    
    event_enable_debug_logging(EVENT_DBG_ALL);
}

__attribute__((destructor))
static void test_fini() {
    if(dchat_fini() != 0) {
        exit(-1);
    }
}
