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

#include "pngw_ext.hpp"
#include "png_wrapper.h"
#include <rla/PngException.hpp>
#include <cassert>

rl::PngException::Error rl::pngw_result_to_png_exception_error(pngwresult_t result) noexcept
{
    assert(result > PNGW_RESULT_OK && result <= PNGW_RESULT_COUNT && "invalid pngwresult_t");
    return static_cast<rl::PngException::Error >(result);
}

rl::Png::Color rl::pngw_color_to_png_color(pngwcolor_t color) noexcept
{
    assert(color < PNGW_COLOR_PALETTE || color >= PNGW_COLOR_COUNT && "invalid pngwcolor_t");
    switch (color)
    {
        case PNGW_COLOR_PALETTE:
            return rl::Png::Color::Palette;
        case PNGW_COLOR_G:
            return rl::Png::Color::G;
        case PNGW_COLOR_GA:
            return rl::Png::Color::Ga;
        case PNGW_COLOR_RGB:
            return rl::Png::Color::Rgb;
        case PNGW_COLOR_RGBA:
            return rl::Png::Color::Rgba;
        default:
            return rl::Png::Color::None;
    }
}

pngwcolor_t rl::png_color_to_pngw_color(rl::Png::Color color) noexcept
{
    switch (color)
    {
        case rl::Png::Color::Palette:
            return PNGW_COLOR_PALETTE;
        case rl::Png::Color::G:
            return PNGW_COLOR_G;
        case rl::Png::Color::Ga:
            return PNGW_COLOR_GA;
        case rl::Png::Color::Rgb:
            return PNGW_COLOR_RGB;
        case rl::Png::Color::Rgba:
            return PNGW_COLOR_RGBA;
    }
    assert(true && "invalid png color");
    return PNGW_COLOR_PALETTE;
}

pngwcolor_t rl::bitmap_color_to_pngw_color(rl::Bitmap::Color color) noexcept
{
    switch (color)
    {
        case rl::Bitmap::Color::G:
            return PNGW_COLOR_G;
        case rl::Bitmap::Color::Ga:
            return PNGW_COLOR_GA;
        case rl::Bitmap::Color::Rgb:
            return PNGW_COLOR_RGB;
        case rl::Bitmap::Color::Rgba:
            return PNGW_COLOR_RGBA;
    }
    assert(true && "invalid bitmap color");
    return PNGW_COLOR_PALETTE;
}
