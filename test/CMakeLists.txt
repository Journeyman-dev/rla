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

Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)
add_executable(RlaTest "")
add_subdirectory(src)
target_link_libraries(RlaTest
    PUBLIC
        rla::rla
        Catch2::Catch2WithMain
)
set_target_properties(RlaTest
    PROPERTIES
    OUTPUT_NAME "rla_test"
    CXX_STANDARD ${RLA_CXX_STANDARD}
    CXX_STANDARD_REQUIRED TRUE
)
include(CTest)
include(Catch)
catch_discover_tests(
    RlaTest
  TEST_PREFIX "unittests."
  REPORTER junit
  OUTPUT_DIR ${CMAKE_BINARY_DIR}
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
)