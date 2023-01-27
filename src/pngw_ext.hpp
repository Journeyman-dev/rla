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

#ifndef RLA_PNGW_EXT_HPP
#define RLA_PNGW_EXT_HPP

#include <rla/Png.hpp>
#include <rla/PngException.hpp>
#include <rla/Bitmap.hpp>
#include "png_wrapper.h"

namespace rl
{
    rl::PngException::Error pngw_result_to_png_exception_error(pngwresult_t result) noexcept;
    rl::Png::Color pngw_color_to_png_color(pngwcolor_t color) noexcept;
    pngwcolor_t png_color_to_pngw_color(rl::Png::Color color) noexcept;
    pngwcolor_t bitmap_color_to_pngw_color(rl::Bitmap::Color color) noexcept;
}

#endif
