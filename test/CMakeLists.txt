# Copyright (C) 2025 Aleksa Radomirovic
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

include_directories(../src)
link_libraries(decentchat ${LIBEVENT_LIBRARIES})

add_executable(test-serverinit serverinit.c)
add_test(NAME serverinit COMMAND test-serverinit)

add_executable(test-portinit portinit.c)
add_test(NAME portinit COMMAND test-portinit)
