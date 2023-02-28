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

#include <rla/MutableBitmapRow.hpp>
#include <rla/bitmap_types.hpp>
#include <vector>

namespace rl
{
    class ImageRow : public rl::MutableBitmapRow
    {
    private:
        std::vector<rl::bitmap_byte_t> data = std::vector<rl::bitmap_byte_t>();
        std::size_t width = 0;
        rl::BitmapDepth depth = rl::BitmapDepth::Default;
        rl::BitmapColor color = rl::BitmapColor::Default;

    public:
        using rl::MutableBitmapRow::MutableBitmapRow;

        std::size_t GetWidth() const noexcept override;
        rl::BitmapDepth GetDepth() const noexcept override;
        rl::BitmapColor GetColor() const noexcept override;
        const rl::bitmap_byte_t* GetData() const noexcept override;
        rl::bitmap_byte_t* GetMutableData() noexcept override;
        void Clear() noexcept;
        void ShrinkToFit();
        void Reserve(std::size_t capacity);
        std::size_t GetCapacity() const noexcept;
        void Create(std::size_t width, rl::BitmapDepth depth, rl::BitmapColor color);

    };
}