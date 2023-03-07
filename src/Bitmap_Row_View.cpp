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
#include <rld/except.hpp>
#include <cstddef>

rl::Bitmap::Row::View::View(
    const rl::Bitmap::byte_t* data,
    std::size_t width,
    rl::Bitmap::Depth depth,
    rl::Bitmap::Color color
) noexcept
    : data(data)
    , width(width)
    , depth(depth)
    , color(color)
{
}

rl::Bitmap::Row::View::View(const rl::Bitmap::Row& row) noexcept
    : data(row.GetData())
    , width(row.GetWidth())
    , depth(row.GetDepth())
    , color(row.GetColor())
{
}

rl::Bitmap::Row::View& rl::Bitmap::Row::View::operator=(const rl::Bitmap::Row& row) noexcept
{
    this->data = row.GetData();
    this->width = row.GetWidth();
    this->depth = row.GetDepth();
    this->color = row.GetColor();
    return *this;
}

std::size_t rl::Bitmap::Row::View::GetWidth() const noexcept
{
    return this->width;
}

rl::Bitmap::Depth rl::Bitmap::Row::View::GetDepth() const noexcept
{
    return this->depth;
}

rl::Bitmap::Color rl::Bitmap::Row::View::GetColor() const noexcept
{
    return this->color;
}

const rl::Bitmap::byte_t* rl::Bitmap::Row::View::GetData() const noexcept
{
    return this->data;
}

const rl::Bitmap::byte_t* rl::Bitmap::Row::View::GetData(std::size_t x, std::size_t channel) const noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return 
        this->data +
        byte_index_o.value();
}

std::size_t rl::Bitmap::Row::View::GetChannelSize() const noexcept
{
    return rl::Bitmap::GetChannelSize(this->depth);
}

std::size_t rl::Bitmap::Row::View::GetBitDepth() const noexcept
{
    return rl::Bitmap::GetBitDepth(this->depth);
}

std::size_t rl::Bitmap::Row::View::GetSize() const noexcept
{
    return rl::Bitmap::GetRowSize(this->width, this->depth, this->color);
}

std::optional<std::size_t> rl::Bitmap::Row::View::GetByteIndex(std::size_t x, std::size_t channel) const noexcept
{
    return
        rl::Bitmap::GetByteIndex(
            this->width,
            1,
            1,
            this->depth,
            this->color,
            0,
            0,
            x,
            0,
            0,
            channel
        );
}