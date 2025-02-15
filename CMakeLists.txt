# Copyright (C) 2025 Aleksa Radomirovic
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.25)
project(decentchat LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3 -Wall")

add_compile_options(
    -g
)

enable_testing()

add_library(
    decentchat SHARED
    src/conn.c
    src/conn.h
    src/server.c
    src/server.h
    src/session.c
    src/session.h
)
target_include_directories(
    decentchat PUBLIC include
)
target_link_libraries(
    decentchat PRIVATE
#   gnutls
#   ngtcp2
#   nghttp3
#   event
    pthread
)

add_library(dchat_test INTERFACE)
target_include_directories(
    dchat_test INTERFACE include src
)
target_link_libraries(
    dchat_test INTERFACE decentchat
)

function(dchat_add_test NAME SOURCE)
    set(EXECNAME ${CMAKE_CURRENT_BINARY_DIR}/${NAME})

    add_executable(
        ${NAME}
        ${SOURCE}
    )
    target_link_libraries(
        ${NAME} PRIVATE dchat_test
    )

    add_test(
        NAME ${NAME}
        COMMAND ${EXECNAME}
        ${ARGV}
    )
    add_test(
        NAME ${NAME}_memcheck
        COMMAND valgrind --verbose --tool=memcheck --leak-check=full --error-exitcode=1 ${EXECNAME}
    )

endfunction()

dchat_add_test(
    dchat_test_session_create
    test/session_create.c
)
dchat_add_test(
    dchat_test_session_create_mult
    test/session_create_mult.c
)
dchat_add_test(
    dchat_test_server_create
    test/server_create.c
)
dchat_add_test(
    dchat_test_server_create_mult
    test/server_create_mult.c
)
dchat_add_test(
    dchat_test_server_create_dup
    test/server_create_dup.c
)
dchat_add_test(
    dchat_test_server_create_dup_session
    test/server_create_dup_session.c
)
dchat_add_test(
    dchat_test_session_destroy
    test/session_destroy.c
)
