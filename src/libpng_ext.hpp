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

#include <rla/PngColor.hpp>
#include <rla/bitmap_types.hpp>
#include <png.h>
#include <fstream>
#include <string>

#define RL_PNG_SIGNATURE_SIZE 8

namespace rl
{
    rl::PngColor libpng_color_to_png_color(int png_color) noexcept;
    int bitmap_color_to_libpng_color(rl::BitmapColor bitmap_color) noexcept;
    void libpng_read_close(png_structp& png_ptr, png_infop& info_ptr, std::ifstream& file);
    void libpng_read_open(std::string_view path, png_structp& png_ptr, png_infop& info_ptr, std::ifstream& file);
    void libpng_set_read_fn(png_structp& png_ptr, std::ifstream& file);
    void libpng_read_file_info(png_structp& png_ptr, png_infop& info_ptr, png_uint_32& png_width, png_uint_32& png_height, int& png_bit_depth, int& png_color_type);
    void libpng_read_configure(png_structp& png_ptr, png_infop& info_ptr, int png_bit_depth, int png_color_type, rl::BitmapDepth depth, rl::BitmapColor color);
    void libpng_write_open(std::string_view path, png_structp& png_ptr, png_infop& info_ptr, std::ofstream& file);
    void libpng_write_close(png_structp& png_ptr, png_infop& info_ptr, std::ofstream& file);
    void libpng_set_write_fn(png_structp& png_ptr, std::ofstream& file);
}