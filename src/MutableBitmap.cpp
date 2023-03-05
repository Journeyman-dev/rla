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

#include <rla/BitmapRowView.hpp>
#include <rla/MutableBitmap.hpp>
#include <rla/MutableBitmapView.hpp>
#include <rla/MutableBitmapRowView.hpp>
#include <rla/ImageRow.hpp>
#include <rla/Png.hpp>
#include <rlm/cellular/cell_box2.hpp>
#include <rlm/cellular/does_contain.hpp>
#include <rld/except.hpp>
#include <rla/color_conversion.hpp>
#include <rld/log.hpp>
#include <png.h>
#include <array>
#include <cstring>
#include <fstream>
#include <optional>
#include "libpng_ext.hpp"
#include <rla/color_conversion.hpp>

bool rl::MutableBitmap::blit_fits(const rl::cell_box2<int>& blit_box, std::size_t page) const noexcept
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

unsigned char* rl::MutableBitmap::GetMutableData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetMutableData() +
        byte_index_o.value();
}

rl::MutableBitmapView rl::MutableBitmap::GetMutableView() noexcept
{
    return
        this->GetMutableView(
            0,
            0,
            0,
            this->GetWidth(),
            this->GetHeight(),
            this->GetPageCount()
        );
}

rl::MutableBitmapView rl::MutableBitmap::GetMutableView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count) noexcept
{
    return
        rl::MutableBitmapView(
            this->GetMutableData(x, y, page, 0),
            width,
            height,
            page_count,
            this->GetDepth(),
            this->GetColor(),
            this->GetRowOffset(),
            this->GetPageOffset()
        );
}

rl::MutableBitmapRowView rl::MutableBitmap::GetMutableRowView(std::size_t y, std::size_t page) noexcept
{
    return
        rl::MutableBitmapRowView(
            this->GetMutableData(0, y, page, 0),
            this->GetWidth(),
            this->GetDepth(),
            this->GetColor()
        );
}

rl::MutableBitmapRowView rl::MutableBitmap::GetModifiedMutableRowView(std::size_t y, std::size_t page, std::optional<rl::BitmapDepth> depth_o, std::optional<rl::BitmapColor> color_o)
{
    if (
        rl::Bitmap::GetPixelSize(depth_o.value_or(this->GetDepth()), color_o.value_or(this->GetColor())) > this->GetPixelSize()
    )
    {
        throw rl::runtime_error("modified row is larger");
    }
    return
        rl::MutableBitmapRowView(
            this->GetMutableData(0, y, page, 0),
            this->GetWidth(),
            depth_o.value_or(this->GetDepth()),
            color_o.value_or(this->GetColor())
        );
}

void rl::MutableBitmap::BlitBitmap(const rl::Bitmap& bitmap, std::size_t x, std::size_t y, std::size_t page)
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
            auto destination_row = this->GetMutableRowView(blit_y, blit_page);
            destination_row.BlitRow(source_row);
        }
    }
}

void rl::MutableBitmap::BlitPng(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page)
{
    this->BlitPng(png.GetPath(), x, y, page);    
}

void rl::MutableBitmap::BlitPng(std::string_view path, std::size_t x, std::size_t y, std::size_t page)
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
        if (this->GetDepth() == rl::BitmapDepth::Normalized)
        {
            // load the image as sexdecuple. will convert the pixels to normalized depth as we go.
            rl::libpng_read_configure(png_ptr, info_ptr, png_bit_depth, png_color_type, rl::BitmapDepth::Sexdecuple, this->GetColor());
            for (size_t y = 0; y < png_height; y++)
            {
                // use a pointer to the row but pretend it has sexdecuple depth
                auto source_row = this->GetModifiedMutableRowView(y, page, rl::BitmapDepth::Sexdecuple, this->GetColor());
                // load the 16 bit pixels in place instead of allocating seperate memory to decrease allocations
                png_read_row(png_ptr, source_row.GetMutableData(), NULL);
                // now get a mutable view to the row with the correct depth
                auto destination_row = this->GetMutableRowView(y, page);
                // blit the row to itself, converting the depth to be normalized
                destination_row.BlitRow(source_row);
            }
        }
        // libpng can load all other situations for us
        else
        {
            rl::libpng_read_configure(png_ptr, info_ptr, png_bit_depth, png_color_type, this->GetDepth(), this->GetColor());
            for (size_t y = 0; y < png_height; y++)
            {
                // grab the row
                auto row = this->GetMutableRowView(y, page);
                // write the pixels to it
                png_read_row(png_ptr, row.GetMutableData(), NULL);
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
