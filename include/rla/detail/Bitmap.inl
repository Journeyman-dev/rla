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
#include <rla/BitmapColor.hpp>
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
        default:
            return 4;
    }
}

constexpr std::size_t rl::Bitmap::GetChannelSize(std::size_t bit_depth) noexcept
{
    const std::size_t bytes_per_pixel = 8;
    return bit_depth / bytes_per_pixel;
}

constexpr std::size_t rl::Bitmap::GetBitDepth(std::size_t channel_size) noexcept
{
    const std::size_t bytes_per_pixel = 8;
    return channel_size * bytes_per_pixel;
}

constexpr std::size_t rl::Bitmap::GetPixelSize(std::size_t channel_size, rl::BitmapColor color) noexcept
{
    return
        channel_size *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetRowSize(std::size_t width, std::size_t channel_size, rl::BitmapColor color) noexcept
{
    return
        width *
        channel_size *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetPageSize(std::size_t width, std::size_t height, std::size_t channel_size, rl::BitmapColor color) noexcept
{
    return
        width *
        height *
        channel_size *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::size_t rl::Bitmap::GetSize(std::size_t width, std::size_t height, std::size_t pages, std::size_t channel_size, rl::BitmapColor color) noexcept
{
    return
        width *
        height *
        pages *
        channel_size *
        rl::Bitmap::GetChannelCount(color);
}

constexpr std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t width, std::size_t height, std::size_t pages, std::size_t channel_size, rl::BitmapColor color, std::size_t row_offset, std::size_t page_offset, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
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
            rl::Bitmap::GetPixelSize(channel_size, color)
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
            channel_size * 
            channel
        );
}

constexpr void rl::Bitmap::ConvertRow(
    unsigned char* source,
    unsigned char* destination,
    std::size_t width,
    std::size_t source_channel_size,
    rl::BitmapColor source_color,
    std::size_t destination_channel_size,
    rl::BitmapColor destination_color
) noexcept
{
	auto convert_pixel = [&]<typename S, typename D>(std::size_t pixel_i) 
	{
		S* source_ptr = reinterpret_cast<S*>(source);
        D* destination_ptr = reinterpret_cast<D*>(destination);
		std::size_t source_i = 0;
		std::size_t destination_i = 0;
		auto source_to_destination = [&]<typename SP>(const SP& source_pixel) {
			if (destination_color == rl::BitmapColor::G)
			{
				const auto destination_pixel = rl::to_color_g<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.g;
			}
			else if (destination_color == rl::BitmapColor::Ga)
			{
				const auto destination_pixel = rl::to_color_ga<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i++] = destination_pixel.a;
			}
			else if (destination_color == rl::BitmapColor::Rgb)
			{
				const auto destination_pixel = rl::to_color_rgb<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.r;
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i++] = destination_pixel.b;
			}
			else if (destination_color == rl::BitmapColor::Rgba)
			{
				const auto destination_pixel = rl::to_color_rgba<D, S>(source_pixel);
				destination_ptr[destination_i++] = destination_pixel.r;
				destination_ptr[destination_i++] = destination_pixel.g;
				destination_ptr[destination_i++] = destination_pixel.b;
				destination_ptr[destination_i++] = destination_pixel.a;
			}
		};
		for (std::size_t pixel_i = 0; pixel_i < width; pixel_i++)
		{
			if (source_color == rl::BitmapColor::G)
			{
				source_to_destination(
					rl::color_g<S>(
						source_ptr[source_i++]
					)
				);
			}
			else if (source_color == rl::BitmapColor::Ga)
			{
				source_to_destination(
					rl::color_ga<S>(
						source_ptr[source_i++],
						source_ptr[source_i++]
					)
				);
			}
			else if (source_color == rl::BitmapColor::Rgb)
			{
				source_to_destination(
					rl::color_rgb<S>(
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i++]
					)
				);				
			}
			else if (source_color == rl::BitmapColor::Rgba)
			{
				source_to_destination(
					rl::color_rgba<S>(
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i++],
						source_ptr[source_i++]
					)
				);
			}
		}
	};
	switch (source_channel_size)
	{
		case 1:
			switch (destination_channel_size)
			{
				case 1:
					convert_row.template operator()<std::uint8_t, std::uint8_t>();
					break;
				case 2:
					convert_row.template operator()<std::uint8_t, std::uint16_t>();
					break;
				case 4:
					convert_row.template operator()<std::uint8_t, std::uint32_t>();
					break;
			}
			break;
		case 2:
			switch (destination_channel_size)
			{
				case 1:
					convert_row.template operator()<std::uint16_t, std::uint8_t>();
					break;
				case 2:
					convert_row.template operator()<std::uint16_t, std::uint16_t>();
					break;
				case 4:
					convert_row.template operator()<std::uint16_t, std::uint32_t>();
					break;
			}
		case 4:
			switch (destination_channel_size)
			{
				case 1:
					convert_row.template operator()<std::uint32_t, std::uint8_t>();
					break;
				case 2:
					convert_row.template operator()<std::uint32_t, std::uint16_t>();
					break;
				case 4:
					convert_row.template operator()<std::uint32_t, std::uint32_t>();
					break;
			}
	}
}