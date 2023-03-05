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
#include <rla/bitmap_types.hpp>
#include <rla/BitmapView.hpp>
#include <rla/BitmapRowView.hpp>
#include <rld/except.hpp>
#include "libpng_ext.hpp"
#include <png.h>
#include <cstddef>
#include <fstream>

std::size_t rl::Bitmap::GetChannelSize() const noexcept
{
    return
        rl::Bitmap::GetChannelSize(
            this->GetDepth()
        );
}

std::size_t rl::Bitmap::GetBitDepth() const noexcept
{
    return
        rl::Bitmap::GetBitDepth(
            this->GetDepth()
        );
}

const rl::bitmap_byte_t* rl::Bitmap::GetData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetData() +
        byte_index_o.value();
}

std::size_t rl::Bitmap::GetChannelCount() const noexcept
{
    return
        rl::Bitmap::GetChannelCount(
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetRowSize() const noexcept
{
    return
        rl::Bitmap::GetRowSize(
            this->GetWidth(),
            this->GetDepth(),
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetPixelSize() const noexcept
{
    return
        rl::Bitmap::GetPixelSize(
            this->GetDepth(),
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetPageSize() const noexcept
{
    return
        rl::Bitmap::GetPageSize(
            this->GetWidth(),
            this->GetHeight(),
            this->GetDepth(),
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetSize() const noexcept
{
    return
        rl::Bitmap::GetSize(
            this->GetWidth(),
            this->GetHeight(),
            this->GetPageCount(),
            this->GetDepth(),
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetRowOffset() const noexcept
{
    return this->GetRowSize();
}

std::size_t rl::Bitmap::GetPageOffset() const noexcept
{
    return this->GetPageSize();
}

std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    return
        rl::Bitmap::GetByteIndex(
            this->GetWidth(),
            this->GetHeight(),
            this->GetPageCount(),
            this->GetDepth(),
            this->GetColor(),
            this->GetRowOffset(),
            this->GetPageOffset(),
            x,
            y,
            page,
            channel
        );
}

rl::BitmapView rl::Bitmap::GetView() const noexcept
{
    return
        this->GetView(
            0,
            0,
            0,
            this->GetWidth(),
            this->GetHeight(),
            this->GetPageCount()
        );
}

rl::BitmapView rl::Bitmap::GetView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count) const noexcept
{
    return
        rl::BitmapView(
            this->GetData(x, y, page, 0),
            width,
            height,
            page_count,
            this->GetDepth(),
            this->GetColor(),
            this->GetRowOffset(),
            this->GetPageOffset()
        );
}

rl::BitmapRowView rl::Bitmap::GetRowView(std::size_t y, std::size_t page) const noexcept
{
    return
        rl::BitmapRowView(
            this->GetData(0, y, page, 0),
            this->GetWidth(),
            this->GetDepth(),
            this->GetColor()
        );
}

void rl::Bitmap::SavePng(std::string_view path, std::size_t page)
{
    if (page >= this->GetPageCount())
    {
        throw rl::runtime_error("save page out of bitmap");
    }
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    std::ofstream file;
    try
    {
        rl::libpng_write_open(path, png_ptr, info_ptr, file);
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            throw rl::runtime_error("libpng jump buffer called");
        }
        rl::libpng_set_write_fn(png_ptr, file);
        const auto png_color = rl::bitmap_color_to_libpng_color(this->GetColor());
        // if the depth is normalized, need to convert each row while writing to a depth that libpng supports
        if (this->GetDepth() == rl::BitmapDepth::Normalized)
        {
            png_set_IHDR(
                png_ptr,
                info_ptr,
                static_cast<png_uint_32>(this->GetWidth()),
                static_cast<png_uint_32>(this->GetHeight()),
                16,
                png_color,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }

    }
    catch(const std::exception& e)
    {
        rl::libpng_write_close(png_ptr, info_ptr, file);
        throw e;
    }
    
}

bool rl::Bitmap::GetIsEmpty() const noexcept
{
    return this->GetData() == nullptr;
}