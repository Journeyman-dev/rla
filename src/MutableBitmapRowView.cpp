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

#include <rla/MutableBitmapRowView.hpp>
#include <rla/Bitmap.hpp>
#include <rla/BitmapRow.hpp>
#include <rla/bitmap_types.hpp>


rl::MutableBitmapRowView::MutableBitmapRowView(
    rl::bitmap_byte_t* row_start,
    std::size_t width,
    rl::BitmapDepth depth,
    rl::BitmapColor color
) noexcept
    : row_start(row_start)
    , width(width)
    , depth(depth)
    , color(color)
{
}

std::size_t rl::MutableBitmapRowView::GetWidth() const noexcept
{
    return this->width;
}

rl::BitmapDepth rl::MutableBitmapRowView::GetDepth() const noexcept
{
    return this->depth;
}

rl::BitmapColor rl::MutableBitmapRowView::GetColor() const noexcept
{
    return this->color;
}

const rl::bitmap_byte_t* rl::MutableBitmapRowView::GetData() const noexcept
{
    return this->row_start;
}

rl::bitmap_byte_t* rl::MutableBitmapRowView::GetMutableData() noexcept
{
    return this->row_start;
}