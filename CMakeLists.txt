# Copyright (C) 2025 Aleksa Radomirovic
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.25)
project(decentchat LANGUAGES C CXX)

option(DEBUG "Build in debug mode" OFF)

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent>=2.1 libevent_pthreads)
pkg_check_modules(LIBTLS REQUIRED gnutls>=3.7)
pkg_check_modules(LIBQUIC REQUIRED libngtcp2 libngtcp2_crypto_gnutls libnghttp3)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wextra -Og -fanalyzer")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden")

add_library(
    decentchat SHARED
    src/api.c
    src/list.h
    src/map.h
    src/port.c
    src/port.h
    src/server.c
    src/server.h
)
target_include_directories(
    decentchat PUBLIC include
)
target_include_directories(
    decentchat PRIVATE
    ${LIBEVENT_INCLUDE_DIRS}
    ${LIBTLS_INCLUDE_DIRS}
    ${LIBQUIC_INCLUDE_DIRS}
)
target_link_libraries(
    decentchat PRIVATE
    pthread
    ${LIBEVENT_LIBRARIES}
    ${LIBTLS_LIBRARIES}
    ${LIBQUIC_LIBRARIES}
)

if(DEBUG)
    add_subdirectory(test)
endif()
