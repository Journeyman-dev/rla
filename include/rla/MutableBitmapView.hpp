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

#include <rla/MutableBitmap.hpp>
#include <rla/BitmapColor.hpp>
#include <cstddef>
#include <optional>

namespace rl
{
    class MutableBitmapView : public rl::MutableBitmap
    {
        private:
            unsigned char* pixel_data = nullptr;

            std::size_t width = 0;
            std::size_t height = 0;
            std::size_t pages = 0;
            std::size_t channel_size = 1;
            rl::BitmapColor color = rl::BitmapColor::Rgb;
            std::size_t row_offset = 0;
            std::size_t page_offset = 0;

        public:
            constexpr MutableBitmapView() noexcept = default;
            MutableBitmapView(
                unsigned char* pixel_data,
                std::size_t width,
                std::size_t height,
                std::size_t pages,
                std::size_t channel_size,
                rl::BitmapColor color,
                std::optional<std::size_t> row_offset_o = std::nullopt,
                std::optional<std::size_t> page_offset_o = std::nullopt
            ) noexcept;

            using rl::MutableBitmap::MutableBitmap;

            std::size_t GetWidth() const noexcept override;
            std::size_t GetHeight() const noexcept override;
            std::size_t GetPages() const noexcept override;
            std::size_t GetChannelSize() const noexcept override;
            rl::BitmapColor GetColor() const noexcept override;
            std::size_t GetRowOffset() const noexcept override;
            std::size_t GetPageOffset() const noexcept override;
            const unsigned char* GetData() const noexcept override;
            unsigned char* GetMutableData() noexcept override;
    };
}