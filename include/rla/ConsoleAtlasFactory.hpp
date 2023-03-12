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

#include <vector>
#include <rla/Image.hpp>
#include <rla/Font.hpp>
#include <rlm/cellular/Packer.hpp>
#include <rlm/cellular/pack_box.hpp>
#include <rla/console_atlas.hpp>

namespace rl
{
    class ConsoleAtlasFactory
    {
        private:
            std::vector<rl::Image> png_images = std::vector<rl::Image>();
            std::vector<rl::Font> font_sources = std::vector<rl::Font>();
            std::vector<rl::pack_box<int>> pack_boxes = std::vector<rl::pack_box<int>>();
            std::vector<std::size_t> glyph_identifiers = std::vector<std::size_t>();
            rl::Packer<int> packer = rl::Packer<int>();

        public:

            rl::console_atlas Create(const rl::console_atlas::layout& layout); 
    };
}