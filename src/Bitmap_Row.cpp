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

rl::Bitmap::Row::Row(
    rl::Bitmap::byte_t* data,
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

std::size_t rl::Bitmap::Row::GetWidth() const noexcept
{
    return this->width;
}

rl::Bitmap::Depth rl::Bitmap::Row::GetDepth() const noexcept
{
    return this->depth;
}

rl::Bitmap::Color rl::Bitmap::Row::GetColor() const noexcept
{
    return this->color;
}

rl::Bitmap::byte_t* rl::Bitmap::Row::GetData() const noexcept
{
    return this->data;
}

rl::Bitmap::byte_t* rl::Bitmap::Row::GetData(std::size_t x, std::size_t channel) const noexcept
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

std::size_t rl::Bitmap::Row::GetChannelSize() const noexcept
{
    return rl::Bitmap::GetChannelSize(this->depth);
}

std::size_t rl::Bitmap::Row::GetBitDepth() const noexcept
{
    return rl::Bitmap::GetBitDepth(this->depth);
}

std::size_t rl::Bitmap::Row::GetSize() const noexcept
{
    return rl::Bitmap::GetRowSize(this->width, this->depth, this->color);
}

std::optional<std::size_t> rl::Bitmap::Row::GetByteIndex(std::size_t x, std::size_t channel) const noexcept
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

void rl::Bitmap::Row::Blit(const rl::Bitmap::Row::View& row)
{
    if (row.GetWidth() != this->width)
    {
        throw rl::runtime_error("blit row has different width");
    }
    auto convert_row = [&]<typename S, typename D>() 
	{
		auto source_to_destination = [&]<typename SP>(std::size_t x, const SP& source_pixel) 
        {
            std::size_t channel_i = 0;
            D* destination_ptr = reinterpret_cast<D*>(this->GetData(x, 0));
			if (this->color == rl::Bitmap::Color::G)
			{
				const auto destination_pixel = rl::to_color_g<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.g;
			}
			else if (this->color == rl::Bitmap::Color::Ga)
			{
				const auto destination_pixel = rl::to_color_ga<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.g;
				destination_ptr[channel_i] = destination_pixel.a;
			}
			else if (this->color == rl::Bitmap::Color::Rgb)
			{
				const auto destination_pixel = rl::to_color_rgb<D, S>(source_pixel);
				destination_ptr[channel_i++] = destination_pixel.r;
				destination_ptr[channel_i++] = destination_pixel.g;
				destination_ptr[channel_i] = destination_pixel.b;
			}
			else if (this->color == rl::Bitmap::Color::Rgba)
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
			if (row.GetColor() == rl::Bitmap::Color::G)
			{
				source_to_destination(
					x,
					rl::color_g<S>(
						source_ptr[channel_i]
					)
				);
			}
			else if (row.GetColor() == rl::Bitmap::Color::Ga)
			{
				source_to_destination(
					x,
					rl::color_ga<S>(
						source_ptr[channel_i++],
						source_ptr[channel_i]
					)
				);
			}
			else if (row.GetColor() == rl::Bitmap::Color::Rgb)
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
			else if (row.GetColor() == rl::Bitmap::Color::Rgba)
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
		case rl::Bitmap::Depth::Octuple:
			switch (this->depth)
			{
				case rl::Bitmap::Depth::Octuple:
					convert_row.template operator()<rl::Bitmap::octuple_t, rl::Bitmap::octuple_t>();
					break;
				case rl::Bitmap::Depth::Sexdecuple:
					convert_row.template operator()<rl::Bitmap::octuple_t, rl::Bitmap::sexdecuple_t>();
					break;
				case rl::Bitmap::Depth::Normalized:
					convert_row.template operator()<rl::Bitmap::octuple_t, rl::Bitmap::normalized_t>();
					break;
			}
			break;
		case rl::Bitmap::Depth::Sexdecuple:
			switch (this->depth)
			{
				case rl::Bitmap::Depth::Octuple:
					convert_row.template operator()<rl::Bitmap::sexdecuple_t, rl::Bitmap::octuple_t>();
					break;
				case rl::Bitmap::Depth::Sexdecuple:
					convert_row.template operator()<rl::Bitmap::sexdecuple_t, rl::Bitmap::sexdecuple_t>();
					break;
				case rl::Bitmap::Depth::Normalized:
					convert_row.template operator()<rl::Bitmap::sexdecuple_t, rl::Bitmap::normalized_t>();
					break;
			}
			break;
		case rl::Bitmap::Depth::Normalized:
			switch (this->depth)
			{
				case rl::Bitmap::Depth::Octuple:
					convert_row.template operator()<rl::Bitmap::normalized_t, rl::Bitmap::octuple_t>();
					break;
				case rl::Bitmap::Depth::Sexdecuple:
					convert_row.template operator()<rl::Bitmap::normalized_t, rl::Bitmap::sexdecuple_t>();
					break;
				case rl::Bitmap::Depth::Normalized:
					convert_row.template operator()<rl::Bitmap::normalized_t, rl::Bitmap::normalized_t>();
					break;
			}
	}
}