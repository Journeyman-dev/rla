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
#include <rla/types.hpp>
#include <rla/Bitmap.hpp>

#include <vector>
#include <cstddef>
#include <optional>

namespace rl
{
    struct console_atlas_layout
    {
        enum class SourceType
        {
            Bitmap,
            Png,
            Font
        };

        struct glyph
        {
            std::size_t source_i = 0;
            rl::console_atlas_layout::SourceType source_type = rl::console_atlas_layout::SourceType::Png;
            std::optional<rl::cell_vector2> top_left_o;
            std::optional<rl::codepoint_t> codepoint_o;
        };

        struct face
        {
            bool letterboxed_wide = false;
            bool letterboxed_tall = false;
            std::optional<std::size_t> font_i_o;
            std::vector<rl::console_atlas_layout::glyph> glyphs;
        };

        std::vector<rl::Bitmap> bitmap_sources;
        std::vector<std::string> png_sources;
        std::vector<std::string> font_sources;
        int tile_width;
        int tile_height;
        std::vector<rl::console_atlas_layout::face> faces;
    };
}

