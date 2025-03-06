# Copyright (C) 2025 Aleksa Radomirovic
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(GTest REQUIRED)

include_directories(
    ../src
    ${LIBEVENT_INCLUDE_DIRS}
    ${LIBTLS_INCLUDE_DIRS}
    ${LIBQUIC_INCLUDE_DIRS}
)
link_libraries(
    GTest::gtest
    GTest::gtest_main

    decentchat

    ${LIBEVENT_LIBRARIES}
    ${LIBTLS_LIBRARIES}
    ${LIBQUIC_LIBRARIES}
)

function(add_gtest NAME)
    add_executable(${NAME} ${NAME}.cpp)
    add_custom_target(test-${NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${NAME} DEPENDS ${NAME})
    list(APPEND ALL_TEST test-${NAME})
    set(ALL_TEST ${ALL_TEST} PARENT_SCOPE)
endfunction()

add_gtest(servertest)

message(STATUS ${ALL_TEST})

add_custom_target(
    test DEPENDS ${ALL_TEST}
)
