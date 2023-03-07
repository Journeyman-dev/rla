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
#include <rla/Image.hpp>
#include <rld/except.hpp>
#include "libpng_ext.hpp"
#include <png.h>

void rl::Bitmap::View::Save(std::string_view path, std::size_t page)
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
        const auto png_color = rl::bitmap_color_to_libpng_color(this->color);
        // if the depth is normalized, need to convert each row while writing to a depth that libpng supports
        if (this->depth == rl::Bitmap::Depth::Normalized)
        {
            const auto write_depth = rl::Bitmap::Depth::Sexdecuple;
            rl::Image::Row convert_row(this->width, write_depth, this->color);
            png_set_IHDR(
                png_ptr,
                info_ptr,
                static_cast<png_uint_32>(this->width),
                static_cast<png_uint_32>(this->height),
                rl::Bitmap::GetBitDepth(write_depth),
                png_color,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
            );
            for (std::size_t row_i = 0; row_i < this->height; row_i++)
            {
                const auto source_row = this->GetRowView(row_i, page);
                convert_row.Blit(source_row);
                png_write_row(png_ptr, reinterpret_cast<png_const_bytep>(convert_row.GetData()));
            }
        }
        else // if (this->depth != rl::Bitmap::Depth::Normalized)
        {
            png_set_IHDR(
                png_ptr,
                info_ptr,
                static_cast<png_uint_32>(this->width),
                static_cast<png_uint_32>(this->height),
                this->GetBitDepth(),
                png_color,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
            );
            for (std::size_t row_i = 0; row_i < this->height; row_i++)
            {
                const auto source_row = this->GetRowView(row_i, page);
                png_write_row(png_ptr, reinterpret_cast<png_const_bytep>(source_row.GetData()));
            }
        }
        rl::libpng_write_close(png_ptr, info_ptr, file);
    }
    catch(const std::exception& e)
    {
        rl::libpng_write_close(png_ptr, info_ptr, file);
        throw e;
    }
}