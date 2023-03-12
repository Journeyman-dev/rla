// SPDX-FileCopyrightText: 2023 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

/*
    Copyright (c) 2023 Daniel Aimé Valcour
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <rlm/cellular/cell_vector2.hpp>
#include <rlm/cellular/hash.hpp>
#include <rlm/hash_combine.hpp>
#include <functional>

namespace rl
{
    struct console_atlas_source_key
    {
        std::size_t source_i;
        rl::cell_vector2<int> top_left;
        bool letterboxed;
        rl::console_atlas::layout::Source source;
        int codepoint;

        bool operator==(const rl::console_atlas_source_key& that) const
        {
            return
                this->source_i == that.source_i &&
                this->top_left == that.top_left &&
                this->letterboxed == that.letterboxed &&
                this->source == that.source &&
                this->codepoint == that.codepoint;
        }
    };
}

namespace std
{
    template<>
    struct hash<rl::console_atlas_source_key> 
    {
        std::size_t operator()(const rl::console_atlas_source_key &source) const
        {
            return
                rl::hash_combine(
                    std::hash<std::size_t>{}(source.source_i),
                    std::hash<rl::cell_vector2<int>>{}(source.top_left),
                    std::hash<bool>{}(source.letterboxed),
                    std::hash<int>{}(static_cast<int>(source.source)),
                    std::hash<int>{}(source.codepoint)
                );
        }
    };
}
