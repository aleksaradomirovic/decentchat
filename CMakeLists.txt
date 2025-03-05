# Copyright (C) 2025 Aleksa Radomirovic
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.25)
project(decentchat LANGUAGES C)

option(DEBUG "Build in debug mode" OFF)

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    option(BUILD_TESTING "Build tests" ON)
else()
    set(CMAKE_BUILD_TYPE Release)
    option(BUILD_TESTING "Build tests" OFF)
endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent>=2.1 libevent_pthreads)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -fanalyzer")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden")

add_library(
    decentchat SHARED
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
)
target_link_libraries(
    decentchat PRIVATE
    pthread
    ${LIBEVENT_LIBRARIES}
)

if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(test)
endif()
