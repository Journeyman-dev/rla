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

#include <rlm/concepts.hpp>
#include <rlm/color/color_g.hpp>
#include <rlm/color/color_ga.hpp>
#include <rlm/color/color_rgb.hpp>
#include <rlm/color/color_rgba.hpp>
#include <rlm/color/color_conversion.hpp>
#include <cstddef>
#include <optional>
#include <cstring>

constexpr std::size_t rl::Bitmap::GetChannelCount(rl::Bitmap::Color color) noexcept
{
    switch (color)
    {
        case rl::Bitmap::Color::G:
            return 1;
        case rl::Bitmap::Color::Ga:
            return 2;
        case rl::Bitmap::Color::Rgb:
            return 3;
        case rl::Bitmap::Color::Rgba:
			return 4;
    }
	return 3;
}

constexpr std::size_t rl::Bitmap::GetChannelSize(rl::Bitmap::Depth depth) noexcept
{
    switch (depth)
	{
		case rl::Bitmap::Depth::Octuple:
			return 1;
		case rl::Bitmap::Depth::Sexdecuple:
			return 2;
		case rl::Bitmap::Depth::Normalized:
			return 4;
	}
	return 1;
}

constexpr std::size_t rl::Bitmap::GetBitDepth(rl::Bitmap::Depth depth) noexcept
{
    return rl::Bitmap::GetChannelSize(depth) * 8;
}

constexpr rl::Bitmap::Depth rl::Bitmap::GetDepth(std::size_t bit_depth) noexcept
{
	switch (bit_depth)
	{
		case 8:
			return rl::Bitmap::Depth::Octuple;
		case 16:
			return rl::Bitmap::Depth::Sexdecuple;
	}
	return rl::Bitmap::Depth::Octuple;
}

constexpr std::size_t rl::Bitmap::GetPixelSize(rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept
{
    return
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetRowSize(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept
{
    return
        width *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetPageSize(std::size_t width, std::size_t height, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept
{
    return
        width *
        height *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetSize(std::size_t width, std::size_t height, std::size_t pages, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept
{
    return
        width *
        height *
        pages *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t width, std::size_t height, std::size_t pages, rl::Bitmap::Depth depth, rl::Bitmap::Color color, std::size_t row_offset, std::size_t page_offset, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
{
    if (
        channel >= rl::Bitmap::GetChannelCount(color) ||
        x >= width ||
        y >= height ||
        page >= pages
    )
    {
        return std::nullopt;
    }
    return
        (
            x *
            rl::Bitmap::GetPixelSize(depth, color)
        ) +
        (
            y *
            row_offset
        ) +
        (
            page *
            page_offset
        ) +
        (
            rl::Bitmap::GetChannelSize(depth) * 
            channel
        );
}

constexpr rl::Bitmap::Bitmap(
    rl::Bitmap::byte_t* data,
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
    , row_offset(row_offset)
{
}

constexpr std::size_t rl::Bitmap::GetWidth() const noexcept
{
    return this->width;
}

constexpr std::size_t rl::Bitmap::GetHeight() const noexcept
{
    return this->height;
}

constexpr std::size_t rl::Bitmap::GetPageCount() const noexcept
{
    return this->page_count;
}

constexpr std::size_t rl::Bitmap::GetChannelSize() const noexcept
{
    return
        rl::Bitmap::GetChannelSize(
            this->depth
        );
}

constexpr rl::Bitmap::Depth rl::Bitmap::GetDepth() const noexcept
{
    return this->depth;
}

constexpr rl::Bitmap::Color rl::Bitmap::GetColor() const noexcept
{
    return this->color;
}

constexpr std::size_t rl::Bitmap::GetBitDepth() const noexcept
{
    return
        rl::Bitmap::GetBitDepth(
            this->depth
        );
}

constexpr rl::Bitmap::byte_t* rl::Bitmap::GetData() const noexcept
{
    return this->data;
}

constexpr rl::Bitmap::byte_t* rl::Bitmap::GetData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->data +
        byte_index_o.value();
}

constexpr std::size_t rl::Bitmap::GetChannelCount() const noexcept
{
    return
        rl::Bitmap::GetChannelCount(
            this->color
        );
}

constexpr std::size_t rl::Bitmap::GetRowSize() const noexcept
{
    return
        rl::Bitmap::GetRowSize(
            this->width,
            this->depth,
            this->color
        );
}

constexpr std::size_t rl::Bitmap::GetPixelSize() const noexcept
{
    return
        rl::Bitmap::GetPixelSize(
            this->depth,
            this->color
        );
}

constexpr std::size_t rl::Bitmap::GetPageSize() const noexcept
{
    return
        rl::Bitmap::GetPageSize(
            this->width,
            this->height,
            this->depth,
            this->color
        );
}

constexpr std::size_t rl::Bitmap::GetSize() const noexcept
{
    return
        rl::Bitmap::GetSize(
            this->width,
            this->height,
            this->page_count,
            this->depth,
            this->color
        );
}

constexpr std::size_t rl::Bitmap::GetRowOffset() const noexcept
{
    return this->row_offset;
}

constexpr std::size_t rl::Bitmap::GetPageOffset() const noexcept
{
    return this->page_offset;
}

constexpr std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
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

constexpr rl::Bitmap rl::Bitmap::GetBitmap(std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o)
{
    return
        this->GetBitmap(
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

constexpr rl::Bitmap rl::Bitmap::GetBitmap(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o)
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
        rl::Bitmap(
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

constexpr rl::Bitmap::View rl::Bitmap::GetBitmapView(std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

constexpr rl::Bitmap::View rl::Bitmap::GetBitmapView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

constexpr rl::Bitmap::Row rl::Bitmap::GetRow(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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
        rl::Bitmap::Row(
            this->GetData(0, y, page, 0),
            this->width,
            fake_depth_o.value_or(this->depth),
            fake_color_o.value_or(this->color)
        );
}

constexpr const rl::Bitmap::Row::View rl::Bitmap::GetRowView(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

constexpr bool rl::Bitmap::GetIsEmpty() const noexcept
{
    return this->width == 0;
}