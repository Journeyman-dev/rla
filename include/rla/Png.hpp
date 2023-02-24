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

#include <rla/PngColor.hpp>
#include <string>
#include <cstddef>

namespace rl
{
    class Png
    {
        private:
            std::string path = "";
            std::size_t width = 0;
            std::size_t height = 0;
            rl::PngColor color = rl::PngColor::None;
            std::size_t bit_depth = 0;

        public:
            constexpr Png() noexcept = default;
            Png(std::string_view path);

            void Load(std::string_view path);
            bool GetEmpty() const noexcept;
            std::string_view GetPath() const noexcept;
            std::size_t GetWidth() const noexcept;
            std::size_t GetHeight() const noexcept;
            std::size_t GetBitDepth() const noexcept;
            rl::PngColor GetColor() const noexcept;
            void Clear() noexcept;
    };
}