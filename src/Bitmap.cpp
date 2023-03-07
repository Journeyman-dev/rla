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
