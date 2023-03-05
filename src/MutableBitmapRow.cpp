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

#include <rla/MutableBitmapRow.hpp>
#include <rla/bitmap_types.hpp>
#include <rld/except.hpp>
#include <rla/Bitmap.hpp>
#include <cstddef>
#include <optional>

rl::bitmap_byte_t* rl::MutableBitmapRow::GetMutableData(std::size_t x, std::size_t channel) noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetMutableData() +
        byte_index_o.value();
}

void rl::MutableBitmapRow::BlitRow(const rl::BitmapRow& row)
{
    if (row.GetWidth() != this->GetWidth())
    {
        throw rl::runtime_error("blit row has different width");
    }
    auto convert_row = [&]<typename S, typename D>() 
	{
		auto source_to_destination = [&]<typename SP>(std::size_t x, const SP& source_pixel) 
        {
            std::size_t channel_i = 0;
            D* destination_ptr = reinterpret_cast<D*>(this->GetMutableData(x, 0));
			if (this->GetColor() == rl::BitmapColor::G)
			{
				const auto destination_pixel = rl::to_color_g<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.g;
			}
			else if (this->GetColor() == rl::BitmapColor::Ga)
			{
				const auto destination_pixel = rl::to_color_ga<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.g;
				destination_ptr[channel_i] = destination_pixel.a;
			}
			else if (this->GetColor() == rl::BitmapColor::Rgb)
			{
				const auto destination_pixel = rl::to_color_rgb<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.r;
				destination_ptr[channel_i++] = destination_pixel.g;
				destination_ptr[channel_i] = destination_pixel.b;
			}
			else if (this->GetColor() == rl::BitmapColor::Rgba)
			{
				const auto destination_pixel = rl::to_color_rgba<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.r;
				destination_ptr[channel_i++] = destination_pixel.g;
				destination_ptr[channel_i++] = destination_pixel.b;
				destination_ptr[channel_i] = destination_pixel.a;
			}
		};
		auto convert_pixel = [&](std::size_t x) 
		{
			const S* source_ptr = reinterpret_cast<const S*>(row.GetData(x, 0));
            std::size_t channel_i = 0;
			if (row.GetColor() == rl::BitmapColor::G)
			{
				source_to_destination(
					x,
					rl::color_g<S>(
						source_ptr[channel_i]
					)
				);
			}
			else if (row.GetColor() == rl::BitmapColor::Ga)
			{
				source_to_destination(
					x,
					rl::color_ga<S>(
						source_ptr[channel_i++],
						source_ptr[channel_i]
					)
				);
			}
			else if (row.GetColor() == rl::BitmapColor::Rgb)
			{
				source_to_destination(
					x,
					rl::color_rgb<S>(
						source_ptr[channel_i++],
						source_ptr[channel_i++],
						source_ptr[channel_i]
					)
				);				
			}
			else if (row.GetColor() == rl::BitmapColor::Rgba)
			{
				source_to_destination(
					x,
					rl::color_rgba<S>(
						source_ptr[channel_i++],
						source_ptr[channel_i++],
						source_ptr[channel_i++],
						source_ptr[channel_i]
					)
				);
			}
		};
		// convert in reverse if necessary to prevent pixel overwriting when the pixels are converted in place. 
		if (sizeof(S) >= sizeof(D))
		{
			for (int pixel_i = 0; pixel_i < this->GetWidth(); pixel_i++)
			{
				convert_pixel(static_cast<std::size_t>(pixel_i));
			}
		}
		else
		{
			for (int pixel_i = this->GetWidth(); pixel_i >= 0; pixel_i--)
			{
				convert_pixel(static_cast<std::size_t>(pixel_i));
			}
		}
	};
	switch (row.GetDepth())
	{
		case rl::BitmapDepth::Octuple:
			switch (this->GetDepth())
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
			switch (this->GetDepth())
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
			switch (this->GetDepth())
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