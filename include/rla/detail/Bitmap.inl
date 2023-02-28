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

constexpr void rl::Bitmap::ConvertRow(
    unsigned char* source,
    unsigned char* destination,
    std::size_t width,
    rl::BitmapDepth source_depth,
    rl::BitmapColor source_color,
    rl::BitmapDepth destination_depth,
    rl::BitmapColor destination_color
) noexcept
{
	auto convert_row = [&]<typename S, typename D>() 
	{
		S* source_ptr = reinterpret_cast<S*>(source);
        D* destination_ptr = reinterpret_cast<D*>(destination);
		auto source_to_destination = [&]<typename SP>(std::size_t pixel_i, const SP& source_pixel) {
			std::size_t destination_i = 
				rl::Bitmap::GetChannelCount(destination_color) *
				pixel_i;
			if (destination_color == rl::BitmapColor::G)
			{
				const auto destination_pixel = rl::to_color_g<D, S>(source_pixel);
				destination_ptr[destination_i] = destination_pixel.g;
			}
			else if (destination_color == rl::BitmapColor::Ga)
			{
				const auto destination_pixel = rl::to_color_ga<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i] = destination_pixel.a;
			}
			else if (destination_color == rl::BitmapColor::Rgb)
			{
				const auto destination_pixel = rl::to_color_rgb<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.r;
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i] = destination_pixel.b;
			}
			else if (destination_color == rl::BitmapColor::Rgba)
			{
				const auto destination_pixel = rl::to_color_rgba<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.r;
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i++] = destination_pixel.b;
				destination_ptr[destination_i] = destination_pixel.a;
			}
		};
		auto convert_pixel = [&](std::size_t pixel_i) 
		{
			std::size_t source_i = 
				rl::Bitmap::GetChannelCount(source_color) *
				pixel_i;
			if (source_color == rl::BitmapColor::G)
			{
				source_to_destination(
					pixel_i,
					rl::color_g<S>(
						source_ptr[source_i]
					)
				);
			}
			else if (source_color == rl::BitmapColor::Ga)
			{
				source_to_destination(
					pixel_i,
					rl::color_ga<S>(
						source_ptr[source_i++],
						source_ptr[source_i]
					)
				);
			}
			else if (source_color == rl::BitmapColor::Rgb)
			{
				source_to_destination(
					pixel_i,
					rl::color_rgb<S>(
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i]
					)
				);				
			}
			else if (source_color == rl::BitmapColor::Rgba)
			{
				source_to_destination(
					pixel_i,
					rl::color_rgba<S>(
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i]
					)
				);
			}
		};
		// convert in reverse if necessary to prevent pixel overwriting when the pixels are converted in place. 
		if (sizeof(S) >= sizeof(D))
		{
			for (int pixel_i = 0; pixel_i < width; pixel_i++)
			{
				convert_pixel(static_cast<std::size_t>(pixel_i));
			}
		}
		else
		{
			for (int pixel_i = width; pixel_i >= 0; pixel_i--)
			{
				convert_pixel(static_cast<std::size_t>(pixel_i));
			}
		}
	};
	switch (source_depth)
	{
		case rl::BitmapDepth::Octuple:
			switch (destination_depth)
			{
				case rl::BitmapDepth::Octuple:
					convert_row.template operator()<std::uint8_t, std::uint8_t>();
					break;
				case rl::BitmapDepth::Sexdecuple:
					convert_row.template operator()<std::uint8_t, std::uint16_t>();
					break;
				case rl::BitmapDepth::Normalized:
					convert_row.template operator()<std::uint8_t, float>();
					break;
			}
			break;
		case rl::BitmapDepth::Sexdecuple:
			switch (destination_depth)
			{
				case rl::BitmapDepth::Octuple:
					convert_row.template operator()<std::uint16_t, std::uint8_t>();
					break;
				case rl::BitmapDepth::Sexdecuple:
					convert_row.template operator()<std::uint16_t, std::uint16_t>();
					break;
				case rl::BitmapDepth::Normalized:
					convert_row.template operator()<std::uint16_t, float>();
					break;
			}
			break;
		case rl::BitmapDepth::Normalized:
			switch (destination_depth)
			{
				case rl::BitmapDepth::Octuple:
					convert_row.template operator()<float, std::uint8_t>();
					break;
				case rl::BitmapDepth::Sexdecuple:
					convert_row.template operator()<float, std::uint16_t>();
					break;
				case rl::BitmapDepth::Normalized:
					convert_row.template operator()<float, float>();
					break;
			}
	}
}