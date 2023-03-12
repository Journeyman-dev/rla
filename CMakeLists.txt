# SPDX-FileCopyrightText: 2023 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

# Copyright (c) 2023 Daniel Aimé Valcour
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.25)

project(rla
    VERSION 0.0.0
    DESCRIPTION "An asset processing library for roguelike game development"
    LANGUAGES CXX
)
set(RLA_CXX_STANDARD 20)
option(RLA_BUILD_TESTS "Enable the automatic test framework for RLA." ON)
add_library(${PROJECT_NAME} STATIC "")
add_subdirectory(src)
add_library(rla::rla ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
Include(FetchContent)
FetchContent_Declare(
    png
    GIT_REPOSITORY https://github.com/glennrp/libpng
    GIT_TAG        v1.6.38
)
FetchContent_Declare(
    rlm
    GIT_REPOSITORY https://github.com/Journeyman-dev/rlm
    GIT_TAG        v0.3.0.3
)
set(RLM_BUILD_TESTS OFF)
FetchContent_Declare(
    rld
    GIT_REPOSITORY https://github.com/Journeyman-dev/rld
    GIT_TAG        v0.0.0
)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype
    GIT_TAG        VER-2-13-0
)
FetchContent_MakeAvailable(png rlm rld freetype)
target_link_libraries(
    ${PROJECT_NAME}
        PUBLIC
            png
            rlm::rlm
            rld::rld
            freetype-interface
)
target_include_directories(
    ${PROJECT_NAME}
        PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME "RLA"
    CXX_STANDARD ${RLA_CXX_STANDARD}
    CXX_STANDARD_REQUIRED TRUE
)
if(RLA_BUILD_TESTS)
    add_subdirectory(test)
endif()