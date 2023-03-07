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
#include <rlm/cellular/cell_box2.hpp>
#include <rlm/cellular/does_contain.hpp>
#include "libpng_ext.hpp"
#include <png.h>
#include <cstddef>
#include <fstream>

bool rl::Bitmap::blit_fits(const rl::cell_box2<int>& blit_box, std::size_t page) const noexcept
{
    rl::cell_box2<int> this_box(
        0,
        0,
        static_cast<int>(this->GetWidth()),
        static_cast<int>(this->GetHeight())
    );
    return 
        page < this->GetPageCount() &&
        rl::does_contain(this_box, blit_box);
}

rl::Bitmap::Bitmap(
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

std::size_t rl::Bitmap::GetWidth() const noexcept
{
    return this->width;
}

std::size_t rl::Bitmap::GetHeight() const noexcept
{
    return this->height;
}

std::size_t rl::Bitmap::GetPageCount() const noexcept
{
    return this->page_count;
}

std::size_t rl::Bitmap::GetChannelSize() const noexcept
{
    return
        rl::Bitmap::GetChannelSize(
            this->depth
        );
}

rl::Bitmap::Depth rl::Bitmap::GetDepth() const noexcept
{
    return this->depth;
}

rl::Bitmap::Color rl::Bitmap::GetColor() const noexcept
{
    return this->color;
}

std::size_t rl::Bitmap::GetBitDepth() const noexcept
{
    return
        rl::Bitmap::GetBitDepth(
            this->depth
        );
}

rl::Bitmap::byte_t* rl::Bitmap::GetData() const noexcept
{
    return this->data;
}

rl::Bitmap::byte_t* rl::Bitmap::GetData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
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

std::size_t rl::Bitmap::GetChannelCount() const noexcept
{
    return
        rl::Bitmap::GetChannelCount(
            this->color
        );
}

std::size_t rl::Bitmap::GetRowSize() const noexcept
{
    return
        rl::Bitmap::GetRowSize(
            this->width,
            this->depth,
            this->color
        );
}

std::size_t rl::Bitmap::GetPixelSize() const noexcept
{
    return
        rl::Bitmap::GetPixelSize(
            this->depth,
            this->color
        );
}

std::size_t rl::Bitmap::GetPageSize() const noexcept
{
    return
        rl::Bitmap::GetPageSize(
            this->width,
            this->height,
            this->depth,
            this->color
        );
}

std::size_t rl::Bitmap::GetSize() const noexcept
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

std::size_t rl::Bitmap::GetRowOffset() const noexcept
{
    return this->row_offset;
}

std::size_t rl::Bitmap::GetPageOffset() const noexcept
{
    return this->page_offset;
}

std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
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

rl::Bitmap rl::Bitmap::GetBitmap(std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o)
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

rl::Bitmap rl::Bitmap::GetBitmap(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o)
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

rl::Bitmap::View rl::Bitmap::GetBitmapView(std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

rl::Bitmap::View rl::Bitmap::GetBitmapView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

rl::Bitmap::Row rl::Bitmap::GetRow(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

const rl::Bitmap::Row::View rl::Bitmap::GetRowView(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o, std::optional<rl::Bitmap::Color> fake_color_o) const
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

bool rl::Bitmap::GetIsEmpty() const noexcept
{
    return this->width == 0;
}

void rl::Bitmap::Save(std::string_view path, std::size_t page)
{
    this->GetBitmapView().Save(path, page);
}

void rl::Bitmap::Blit(const rl::Bitmap::View& bitmap, std::size_t x, std::size_t y, std::size_t page)
{
    if (
        !this->blit_fits(
            rl::cell_box2<int>(
                x,
                y,
                bitmap.GetWidth(),
                bitmap.GetHeight()
            ),
            page
        )
    )
    {
        throw rl::runtime_error("blit out of bitmap");
    }
    for (std::size_t blit_page = 0; blit_page < bitmap.GetPageCount(); blit_page++)
    {
        for (std::size_t blit_y = 0; blit_y < bitmap.GetHeight(); blit_y++)
        {
            const auto source_row = bitmap.GetRowView(blit_y, blit_page);
            auto destination_row = this->GetRow(blit_y, blit_page);
            destination_row.Blit(source_row);
        }
    }
}

void rl::Bitmap::Blit(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page)
{
    this->Blit(png.GetPath(), x, y, page);    
}

void rl::Bitmap::Blit(std::string_view path, std::size_t x, std::size_t y, std::size_t page)
{
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    std::ifstream file;
    try
    {
        rl::libpng_read_open(path, png_ptr, info_ptr, file);
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            throw rl::runtime_error("libpng jump buffer called");
        }
        rl::libpng_set_read_fn(png_ptr, file);
        png_uint_32 png_width, png_height;
        int png_bit_depth, png_color_type;
        rl::libpng_read_file_info(png_ptr, info_ptr, png_width, png_height, png_bit_depth, png_color_type);
        // libpng can not load images with normalized floating point depth, so some manual conversion is required
        if (this->GetDepth() == rl::Bitmap::Depth::Normalized)
        {
            // load the image as sexdecuple. will convert the pixels to normalized depth as we go.
            rl::libpng_read_configure(png_ptr, info_ptr, png_bit_depth, png_color_type, rl::Bitmap::Depth::Sexdecuple, this->color);
            for (size_t y = 0; y < png_height; y++)
            {
                // use a pointer to the row but pretend it has sexdecuple depth
                auto source_row = this->GetRow(y, page, rl::Bitmap::Depth::Sexdecuple);
                // load the 16 bit pixels in place instead of allocating seperate memory to decrease allocations
                png_read_row(png_ptr, reinterpret_cast<png_bytep>(source_row.GetData()), NULL);
                // now get a mutable view to the row with the correct depth
                auto destination_row = this->GetRow(y, page);
                // blit the row to itself, converting the depth to be normalized
                destination_row.Blit(source_row);
            }
        }
        // libpng can load all other situations for us
        else
        {
            rl::libpng_read_configure(png_ptr, info_ptr, png_bit_depth, png_color_type, this->depth, this->color);
            for (size_t y = 0; y < png_height; y++)
            {
                // grab the row
                auto row = this->GetRow(y, page);
                // write the pixels to it
                png_read_row(png_ptr, reinterpret_cast<png_bytep>(row.GetData()), NULL);
            }
        }
    }
    catch(const std::exception& e)
    {
        rl::libpng_read_close(png_ptr, info_ptr, file);
        throw e;
    }
    rl::libpng_read_close(png_ptr, info_ptr, file);
}
