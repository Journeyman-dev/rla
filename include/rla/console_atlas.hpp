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

#include <rla/Image.hpp>
#include <rlm/cellular/cell_vector2.hpp>
#include <string>
#include <vector>
#include <map>

namespace rl
{
    struct console_atlas    
    {
        using glyph_i = std::uint32_t;
        using codepoint_i = std::uint32_t;

        enum class Color
        {
            GGrayStencilFg,
            //GaBlendFgAlphaBg,   // TODO
            //GaGrayFgAlphaBg,    // TODO
            //RgbaBlendFgAlphaBg, // TODO
            Default = GGrayStencilFg
        };

        struct layout
        {
            enum class Source
            {
                Bitmap,
                Png,
                Font
            };

            struct glyph
            {
                std::size_t source_i = 0;
                rl::console_atlas::layout::Source source = rl::console_atlas::layout::Source::Png;
                std::optional<rl::cell_vector2<int>> top_left_o;
                std::optional<rl::console_atlas::codepoint_i> codepoint_o;
            };

            struct face
            {
                bool letterboxed_wide = false;
                bool letterboxed_tall = false;
                std::vector<rl::console_atlas::layout::glyph> glyphs;
            };

            std::vector<rl::Bitmap::View> bitmap_sources;
            std::vector<std::string> png_sources;
            std::vector<std::string> font_sources;
            int tile_width;
            int tile_height;
            rl::console_atlas::Color color = rl::console_atlas::Color::Default;
            std::vector<rl::console_atlas::layout::face> faces;
        };

        struct face
        {
            bool letterboxed_wide = false;
            bool letterboxed_tall = false;
            std::vector<float> texture_coordinates = std::vector<float>();
            std::map<rl::console_atlas::codepoint_i, rl::console_atlas::glyph_i> codepoint_map = std::map<rl::console_atlas::codepoint_i, rl::console_atlas::glyph_i>();
        };

        int tile_width = 0;
        int tile_height = 0;
        bool has_letterboxing_wide = false;
        bool has_letterboxing_tall = false;
        rl::console_atlas::Color color = rl::console_atlas::Color::Default;
        rl::Image image = rl::Image();
        std::vector<rl::console_atlas::face> faces = std::vector<rl::console_atlas::face>();

        static rl::console_atlas Create(const rl::console_atlas::layout& layout);
    };
}