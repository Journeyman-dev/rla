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
option(RLA_AUTO_FETCH "Automatically fetch the dependencies of RLA using FetchContent." ON)
add_library(${PROJECT_NAME} STATIC "")
add_subdirectory(src)
add_library(rla::rla ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
if (RLA_AUTO_FETCH)
    Include(FetchContent)
    FetchContent_Declare(
        png
        GIT_REPOSITORY https://github.com/glennrp/libpng
        GIT_TAG        v1.6.38
    )
    FetchContent_Declare(
        rlm
        GIT_REPOSITORY https://github.com/Journeyman-dev/rlm
        GIT_TAG        v0.1.0.0
    )
    FetchContent_MakeAvailable(png rlm)
    target_link_libraries(
        ${PROJECT_NAME}
            PRIVATE
                png
                rlm
    )
else()
    find_package(PNG REQUIRED)
    target_include_directories(
        ${PROJECT_NAME}
            PRIVATE
                ${PNG_INCLUDE_DIR}
    )
    target_link_libraries(
        ${PROJECT_NAME}
            PRIVATE
                ${PNG_LIBRARY}
    )
endif()
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
add_subdirectory(test)
