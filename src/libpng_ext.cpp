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

#include "libpng_ext.hpp"
#include <rld/except.hpp>
#include <rld/log.hpp>
#include <png.h>
#include <fstream>
#include <array>
#include <string>

rl::PngColor rl::libpng_color_to_png_color(int png_color) noexcept
{
    switch (png_color)
    {
    case PNG_COLOR_TYPE_PALETTE:
      return rl::PngColor::Palette;
    case PNG_COLOR_TYPE_GRAY:
      return rl::PngColor::G;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      return rl::PngColor::Ga;
    case PNG_COLOR_TYPE_RGB:
      return rl::PngColor::Rgb;
    case PNG_COLOR_TYPE_RGBA:
      return rl::PngColor::Rgba;
    }
    return rl::PngColor::None;
}

int rl::bitmap_color_to_libpng_color(rl::BitmapColor color) noexcept
{
    switch (color)
    {
    case rl::BitmapColor::G:
      return PNG_COLOR_TYPE_GRAY;
    case rl::BitmapColor::Ga:
      return PNG_COLOR_TYPE_GRAY_ALPHA;
    case rl::BitmapColor::Rgb:
      return PNG_COLOR_TYPE_RGB;
    case rl::BitmapColor::Rgba:
      return PNG_COLOR_TYPE_RGBA;
    }
    return PNG_COLOR_TYPE_RGB;
}

void rl::libpng_read_close(png_structp& png_ptr, png_infop& info_ptr, std::ifstream& file)
{
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  file.close();
}

void rl::libpng_read_open(std::string_view path, png_structp& png_ptr, png_infop& info_ptr, std::ifstream& file)
{
    file.open(path.data(), std::ios::in);
    std::array<png_byte, RL_PNG_SIGNATURE_SIZE> signature;
    file.read((char*)signature.data(), signature.size());
    if (!file.good())
    {
        throw rl::runtime_error("libpng file open failure");
    }
    if (!png_sig_cmp(signature.data(), 0, RL_PNG_SIGNATURE_SIZE))
    {
        throw rl::runtime_error("libpng png invalid file signiture");
    }
    png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        [](png_structp png_ptr, png_const_charp message)
        {
            rl::error("libpng: {}", message);
        },
        [](png_structp png_ptr, png_const_charp message)
        {
            rl::warn("libpng: {}", message);
        }
    );
    if (png_ptr == nullptr)
    {
        throw rl::runtime_error("libpng png read struct create failure");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr)
    {
        throw rl::runtime_error("libpng info struct create failure");
    }
}

void rl::libpng_set_read_fn(png_structp& png_ptr, std::ifstream& file)
{
    png_set_read_fn(png_ptr, &file,
        [](png_structp png_ptr, png_bytep data, png_size_t length)
        {
            png_voidp io_ptr = png_get_io_ptr(png_ptr);
            auto file_ptr = reinterpret_cast<std::ifstream*>(io_ptr);
            file_ptr->read((char*)data, length);
        }
    );
}

void rl::libpng_read_file_info(png_structp& png_ptr, png_infop& info_ptr, png_uint_32& png_width, png_uint_32& png_height, int& png_bit_depth, int& png_color_type)
{
  png_set_sig_bytes(png_ptr, RL_PNG_SIGNATURE_SIZE);
  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(
      png_ptr,
      info_ptr,
      &png_width,
      &png_height,
      &png_bit_depth,
      &png_color_type,
      nullptr,
      nullptr,
      nullptr
  );
}

void rl::libpng_read_configure(png_structp& png_ptr, png_infop& info_ptr, int png_bit_depth, int png_color_type, rl::BitmapDepth depth, rl::BitmapColor color)
{
  // the following is taken from png_wrapper.h (https://github.com/Journeyman-dev/png_wrapper.h/)
  // if alpha channel not wanted, strip it if the image has one.
  if ((png_color_type & PNG_COLOR_MASK_ALPHA) && color != rl::BitmapColor::Ga && color != rl::BitmapColor::Rgba)
  {
    png_set_strip_alpha(png_ptr);
  }
  // if expecting an alpha channel and none exists in the image, add a fully opaque alpha value to
  // each pixel
  if ((color == rl::BitmapColor::Ga || color == rl::BitmapColor::Rgba) &&
      (png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_RGB ||
        png_color_type == PNG_COLOR_TYPE_PALETTE))
  {
    png_set_filler(png_ptr, 0xffff, PNG_FILLER_AFTER);
  }
  // if multiple pixels are packed per byte on 8 bit depth, seperate them into seperate bytes
  // cleanly
  if (png_bit_depth < 8 && depth == rl::BitmapDepth::Octuple)
  {
    png_set_packing(png_ptr);
  }
  // if the image has bit depth 16 and 8 is wanted, auto convert it to bit depth 8
  if (png_bit_depth == 16 && depth == rl::BitmapDepth::Octuple)
  {
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
#else
    png_set_strip_16(png_ptr);  // if scaling not supported, strip off the excess byte instead
#endif
  }
  // if image has less than 16 bit depth and 16 is wanted, upscale it to 16
  if (png_bit_depth < 16 && depth == rl::BitmapDepth::Sexdecuple)
  {
    png_set_expand_16(png_ptr);
  }
  // if gray and bit depth is less than 8, up it to 8
  if ((png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) &&
      png_bit_depth < 8)
  {
    png_set_expand_gray_1_2_4_to_8(png_ptr);
  }
  // auto convert palette images into rgb or rgba
  if (png_color_type == PNG_COLOR_TYPE_PALETTE)
  {
    png_set_palette_to_rgb(png_ptr);
  }
  // set transparency to full alpha channels
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0)
  {
    png_set_tRNS_to_alpha(png_ptr);
  }
  // set rgb image to gray output or vice versa
  if (
    (
      color == rl::BitmapColor::G ||
      color == rl::BitmapColor::Ga
    ) &&
    (
      png_color_type == PNG_COLOR_TYPE_RGB ||
      png_color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
      png_color_type == PNG_COLOR_TYPE_PALETTE
    )
  )
  {
    // negative weights causes default calculation to be used  ((6969 * R + 23434 * G + 2365 *
    // B)/32768)
    png_set_rgb_to_gray_fixed(
        png_ptr, 1, -1.0,
        -1.0);  // error action 1 causes no waring warning if image was not actually gray
  }
  if ((color == rl::BitmapColor::Rgb || color == rl::BitmapColor::Rgba) &&
    (png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
  {
    png_set_gray_to_rgb(png_ptr);
  }
}

void rl::libpng_write_open(std::string_view path, png_structp& png_ptr, png_infop& info_ptr, std::ofstream& file)
{
    file.open(path.data(), std::ios::out | std::ios::trunc);
    png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        [](png_structp png_ptr, png_const_charp message)
        {
            rl::error("libpng: {}", message);
        },
        [](png_structp png_ptr, png_const_charp message)
        {
            rl::warn("libpng: {}", message);
        }
    );
    if (!png_ptr)
    {
        throw rl::runtime_error("libpng png write struct create failure");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        throw rl::runtime_error("libpng info struct create failure");
    }
}

void rl::libpng_write_close(png_structp& png_ptr, png_infop& info_ptr, std::ofstream& file)
{
  png_destroy_write_struct(&png_ptr, &info_ptr);
  file.close();
}

void rl::libpng_set_write_fn(png_structp& png_ptr, std::ofstream& file)
{
    png_set_write_fn(png_ptr, &file,
        [](png_structp png_ptr, png_bytep data, png_size_t length)
        {
            png_voidp io_ptr = png_get_io_ptr(png_ptr);
            auto file_ptr = reinterpret_cast<std::ofstream*>(io_ptr);
            file_ptr->write((char*)data, length);
        },
        [](png_structp png_ptr){}
    );
}