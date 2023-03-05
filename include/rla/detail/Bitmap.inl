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
#include <rla/bitmap_types.hpp>
#include <rlm/color/color_g.hpp>
#include <rlm/color/color_ga.hpp>
#include <rlm/color/color_rgb.hpp>
#include <rlm/color/color_rgba.hpp>
#include <rlm/color/color_conversion.hpp>
#include <cstddef>
#include <optional>
#include <cstring>

constexpr std::size_t rl::Bitmap::GetChannelCount(rl::BitmapColor color) noexcept
{
    switch (color)
    {
        case rl::BitmapColor::G:
            return 1;
        case rl::BitmapColor::Ga:
            return 2;
        case rl::BitmapColor::Rgb:
            return 3;
        case rl::BitmapColor::Rgba:
			return 4;
    }
	return 3;
}

constexpr std::size_t rl::Bitmap::GetChannelSize(rl::BitmapDepth depth) noexcept
{
    switch (depth)
	{
		case rl::BitmapDepth::Octuple:
			return 1;
		case rl::BitmapDepth::Sexdecuple:
			return 2;
		case rl::BitmapDepth::Normalized:
			return 4;
	}
	return 1;
}

constexpr std::size_t rl::Bitmap::GetBitDepth(rl::BitmapDepth depth) noexcept
{
    return rl::Bitmap::GetChannelSize(depth) * 8;
}

constexpr rl::BitmapDepth rl::Bitmap::GetDepth(std::size_t bit_depth) noexcept
{
	switch (bit_depth)
	{
		case 8:
			return rl::BitmapDepth::Octuple;
		case 16:
			return rl::BitmapDepth::Sexdecuple;
	}
	return rl::BitmapDepth::Octuple;
}

constexpr std::size_t rl::Bitmap::GetPixelSize(rl::BitmapDepth depth, rl::BitmapColor color) noexcept
{
    return
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetRowSize(std::size_t width, rl::BitmapDepth depth, rl::BitmapColor color) noexcept
{
    return
        width *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetPageSize(std::size_t width, std::size_t height, rl::BitmapDepth depth, rl::BitmapColor color) noexcept
{
    return
        width *
        height *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetSize(std::size_t width, std::size_t height, std::size_t pages, rl::BitmapDepth depth, rl::BitmapColor color) noexcept
{
    return
        width *
        height *
        pages *
        rl::Bitmap::GetChannelSize(depth) *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t width, std::size_t height, std::size_t pages, rl::BitmapDepth depth, rl::BitmapColor color, std::size_t row_offset, std::size_t page_offset, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
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