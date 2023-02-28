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

#include <rla/bitmap_types.hpp>
#include <rla/MutableBitmap.hpp>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace rl
{
    class Image : public rl::MutableBitmap
    {
        private:
            std::size_t width = 0;
            std::size_t height = 0;
            std::size_t page_count = 0;
            rl::BitmapDepth depth = rl::BitmapDepth::Default;
            rl::BitmapColor color = rl::BitmapColor::Default;
            std::vector<rl::bitmap_byte_t> data = std::vector<rl::bitmap_byte_t>();

        public:
            using rl::MutableBitmap::MutableBitmap;

            rl::BitmapColor GetColor() const noexcept override;
            std::size_t GetWidth() const noexcept override;
            std::size_t GetHeight() const noexcept override;
            std::size_t GetPageCount() const noexcept override;
            rl::BitmapDepth GetDepth() const noexcept override;
            rl::bitmap_byte_t* GetMutableData() noexcept override;
            const rl::bitmap_byte_t* GetData() const noexcept override;
            void Clear() noexcept;
            void ShrinkToFit();
            void Reserve(std::size_t bytes);
            std::size_t GetCapacity() const noexcept;
            void Create(std::size_t width, std::size_t height, std::size_t page_count, rl::BitmapDepth depth, rl::BitmapColor color);
            void LoadPng(const rl::Png& png, std::optional<rl::BitmapDepth> depth_o = std::nullopt, std::optional<rl::BitmapColor> color_o = std::nullopt);
            void LoadPng(std::string_view path, std::optional<rl::BitmapDepth> depth_o = std::nullopt, std::optional<rl::BitmapColor> color_o = std::nullopt);
    };
}