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

#include <rla/Png.hpp>
#include <rld/except.hpp>
#include "libpng_ext.hpp"
#include <fstream>
#include <array>
#include <string>
#include <png.h>
#include <rld/log.hpp>

rl::Png::Png(std::string_view path)
{
    this->Load(path);
}

void rl::Png::Load(std::string_view path)
{
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    std::ifstream file;
    rl::libpng_read_open(path, png_ptr, info_ptr, file);
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        rl::libpng_read_close(png_ptr, info_ptr, file);
        throw rl::runtime_error("libpng jump buffer called");
    }
    rl::libpng_set_read_fn(png_ptr, file);
    png_uint_32 png_width, png_height;
    int png_bit_depth, png_color_type;
    rl::libpng_read_file_info(png_ptr, info_ptr, png_width, png_height, png_bit_depth, png_color_type);
    rl::libpng_read_close(png_ptr, info_ptr, file);
    this->path = path;
    this->width = static_cast<std::size_t>(png_width);
    this->height = static_cast<std::size_t>(png_height);
    this->color = rl::libpng_color_to_png_color(png_color_type);
    this->bit_depth = static_cast<std::size_t>(bit_depth);
}

bool rl::Png::GetIsLoaded() const noexcept
{
    return this->color == rl::Png::Color::None;
}

std::string_view rl::Png::GetPath() const noexcept
{
    return this->path;
}

std::size_t rl::Png::GetWidth() const noexcept
{
    return this->width;
}

std::size_t rl::Png::GetHeight() const noexcept
{
    return this->height;
}

std::size_t rl::Png::GetBitDepth() const noexcept
{
    return this->bit_depth;
}

void rl::Png::Clear() noexcept
{
    *this = rl::Png();
}

rl::Png::Color rl::Png::GetColor() const noexcept
{
    return this->color;
}