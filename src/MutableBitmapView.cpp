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

#include <rla/Bitmap.hpp>
#include <rla/MutableBitmapView.hpp>
#include <rla/BitmapColor.hpp>
#include <cstddef>
#include <optional>

rl::MutableBitmapView::MutableBitmapView(
    unsigned char* pixel_data,
    std::size_t width,
    std::size_t height,
    std::size_t page_count,
    rl::BitmapDepth depth,
    rl::BitmapColor color,
    std::optional<std::size_t> row_offset_o,
    std::optional<std::size_t> page_offset_o
) noexcept
    : width(width)
    , height(height)
    , page_count(page_count)
    , depth(depth)
    , color(color)
    , row_offset(
        row_offset_o.value_or(
            rl::Bitmap::GetRowSize(width, depth, color)
        )
    )
    , page_offset(
        page_offset_o.value_or(
            rl::Bitmap::GetPageSize(width, height, depth, color)
        )
    )
{
}

std::size_t rl::MutableBitmapView::GetWidth() const noexcept
{
    return this->width;
}

std::size_t rl::MutableBitmapView::GetHeight() const noexcept
{
    return this->height;
}

std::size_t rl::MutableBitmapView::GetPageCount() const noexcept
{
    return this->page_count;
}

rl::BitmapDepth rl::MutableBitmapView::GetDepth() const noexcept
{
    return this->depth;
}

rl::BitmapColor rl::MutableBitmapView::GetColor() const noexcept
{
    return this->color;
}

std::size_t rl::MutableBitmapView::GetRowOffset() const noexcept
{
    return this->row_offset;
}

std::size_t rl::MutableBitmapView::GetPageOffset() const noexcept
{
    return this->page_offset;
}

const unsigned char* rl::MutableBitmapView::GetData() const noexcept
{
    return this->pixel_data;
}

unsigned char* rl::MutableBitmapView::GetMutableData() noexcept
{
    return this->pixel_data;
}
