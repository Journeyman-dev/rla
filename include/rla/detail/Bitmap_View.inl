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

#include <rld/except.hpp>
#include <cstddef>
#include <optional>

constexpr rl::Bitmap::View::View(
    const rl::Bitmap::byte_t* data,
    std::size_t width,
    std::size_t height,
    std::size_t page_count,
    rl::Bitmap::Depth depth,
    rl::Bitmap::Color color,
    std::optional<std::size_t> row_offset_o,
    std::optional<std::size_t> page_offset_o
) noexcept
    : data(data)
    , width(width)
    , height(height)
    , page_count(page_count)
    , depth(depth)
    , color(color)
    , row_offset(
        row_offset_o.value_or(
            rl::Bitmap::GetRowSize(\
                width,
                depth,
                color
            )
        )
    )
    , page_offset(
        page_offset_o.value_or(
            rl::Bitmap::GetPageSize(
                width,
                height,
                depth,
                color
            )
        )
    )
{
}

constexpr rl::Bitmap::View::View(const rl::Bitmap& bitmap) noexcept
    : data(const_cast<rl::Bitmap&>(bitmap).GetData())
    , width(bitmap.GetWidth())
    , height(bitmap.GetHeight())
    , page_count(bitmap.GetPageCount())
    , depth(bitmap.GetDepth())
    , color(bitmap.GetColor())
    , row_offset(bitmap.GetRowOffset())
    , page_offset(bitmap.GetPageOffset())
{
}

constexpr rl::Bitmap::View& rl::Bitmap::View::operator=(const rl::Bitmap& bitmap) noexcept
{
    this->data = bitmap.GetData();
    this->width = bitmap.GetWidth();
    this->height = bitmap.GetHeight();
    this->page_count = bitmap.GetPageCount();
    this->depth = bitmap.GetDepth();
    this->color = bitmap.GetColor();
    this->row_offset = bitmap.GetRowOffset();
    this->page_offset = bitmap.GetPageOffset();
    return *this;
}

constexpr std::size_t rl::Bitmap::View::GetWidth() const noexcept
{
    return this->width;
}

constexpr std::size_t rl::Bitmap::View::GetHeight() const noexcept
{
    return this->height;
}

constexpr std::size_t rl::Bitmap::View::GetPageCount() const noexcept
{
    return this->page_count;
}

constexpr std::size_t rl::Bitmap::View::GetChannelSize() const noexcept
{
    return rl::Bitmap::GetChannelSize(this->depth);
}

constexpr rl::Bitmap::Depth rl::Bitmap::View::GetDepth() const noexcept
{
    return this->depth;
}

constexpr rl::Bitmap::Color rl::Bitmap::View::GetColor() const noexcept
{
    return this->color;
}

constexpr std::size_t rl::Bitmap::View::GetBitDepth() const noexcept
{
    return rl::Bitmap::GetBitDepth(this->depth);
}

constexpr std::size_t rl::Bitmap::View::GetRowOffset() const noexcept
{
    return this->row_offset;
}

constexpr std::size_t rl::Bitmap::View::GetPageOffset() const noexcept
{
    return this->page_offset;
}

constexpr const rl::Bitmap::byte_t* rl::Bitmap::View::GetData() const noexcept
{
    return this->data;
}

constexpr const rl::Bitmap::byte_t* rl::Bitmap::View::GetData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
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

constexpr std::size_t rl::Bitmap::View::GetChannelCount() const noexcept
{
    return rl::Bitmap::GetChannelCount(this->color);
}

constexpr std::size_t rl::Bitmap::View::GetRowSize() const noexcept
{
    return rl::Bitmap::GetRowSize(this->width, this->depth, this->color);
}

constexpr std::size_t rl::Bitmap::View::GetPixelSize() const noexcept
{
    return rl::Bitmap::GetPixelSize(this->depth, this->color);
}

constexpr std::size_t rl::Bitmap::View::GetPageSize() const noexcept
{
    return rl::Bitmap::GetPageSize(this->width, this->height, this->depth, this->color);
}

constexpr std::size_t rl::Bitmap::View::GetSize() const noexcept
{
    return rl::Bitmap::GetSize(this->width, this->height, this->page_count, this->depth, this->color);
}

constexpr std::optional<std::size_t> rl::Bitmap::View::GetByteIndex(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    return
        rl::Bitmap::GetByteIndex(
            this->width,
            this->height,
            this->page_count,
            this->depth,
            this->color,
            this->row_offset,
            this->page_offset,
            x,
            y,
            page,
            channel
        );
}

constexpr bool rl::Bitmap::View::GetIsEmpty() const noexcept
{
    return this->width == 0;
}

constexpr const rl::Bitmap::View rl::Bitmap::View::GetBitmapView(std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
{
    return
        this->GetBitmapView(
            0,
            0,
            0,
            this->width,
            this->height,
            this->page_count,
            fake_depth_o,
            fake_color_o
        );
}

constexpr const rl::Bitmap::View rl::Bitmap::View::GetBitmapView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
{
    if (
        x >= this->width ||
        y >= this->height ||
        page >= this->page_count ||
        x + width >= this->width ||
        y + height >= this->height ||
        page + page_count >= this->page_count
    )
    {
        throw rl::runtime_error("view out of bitmap");
    }
    if (rl::Bitmap::GetRowSize(this->width, fake_depth_o.value_or(this->depth), fake_color_o.value_or(this->color)) > this->GetRowSize())
    {
        throw rl::runtime_error("fake bitmap size larger than real bitmap size");
    }
    return
        rl::Bitmap::View(
            this->GetData(x, y, page, 0),
            width,
            height,
            page_count,
            fake_depth_o.value_or(this->depth),
            fake_color_o.value_or(this->color),
            this->row_offset,
            this->page_offset
        );
}

constexpr const rl::Bitmap::Row::View rl::Bitmap::View::GetRowView(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
{
    if (y >= this->height || page >= this->page_count)
    {
        throw rl::runtime_error("row out of bitmap");
    }
    if (rl::Bitmap::GetRowSize(this->width, fake_depth_o.value_or(this->depth), fake_color_o.value_or(this->color)) > this->GetRowSize())
    {
        throw rl::runtime_error("fake row size larger than real row size");
    }
    return
        rl::Bitmap::Row::View(
            this->GetData(0, y, page, 0),
            this->width,
            fake_depth_o.value_or(this->depth),
            fake_color_o.value_or(this->color)
        );
}