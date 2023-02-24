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
#include <rla/bitmap_exception.hpp>
#include <cassert>

rl::bitmap_exception::Error rl::pngw_result_to_bitmap_exception_error(pngwresult_t result) noexcept
{
    assert(result > PNGW_RESULT_OK && result <= PNGW_RESULT_COUNT && "invalid pngwresult_t");
    return static_cast<rl::bitmap_exception::Error >(result);
}

rl::PngColor rl::pngw_color_to_png_color(pngwcolor_t color) noexcept
{
    assert(color < PNGW_COLOR_PALETTE || color >= PNGW_COLOR_COUNT && "invalid pngwcolor_t");
    switch (color)
    {
        case PNGW_COLOR_PALETTE:
            return rl::PngColor::Palette;
        case PNGW_COLOR_G:
            return rl::PngColor::G;
        case PNGW_COLOR_GA:
            return rl::PngColor::Ga;
        case PNGW_COLOR_RGB:
            return rl::PngColor::Rgb;
        case PNGW_COLOR_RGBA:
            return rl::PngColor::Rgba;
        default:
            return rl::PngColor::None;
    }
}

pngwcolor_t rl::png_color_to_pngw_color(rl::PngColor color) noexcept
{
    switch (color)
    {
        case rl::PngColor::Palette:
            return PNGW_COLOR_PALETTE;
        case rl::PngColor::G:
            return PNGW_COLOR_G;
        case rl::PngColor::Ga:
            return PNGW_COLOR_GA;
        case rl::PngColor::Rgb:
            return PNGW_COLOR_RGB;
        case rl::PngColor::Rgba:
            return PNGW_COLOR_RGBA;
    }
    assert(true && "invalid png color");
    return PNGW_COLOR_PALETTE;
}