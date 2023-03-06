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

#include <rla/Bitmap.hpp>
#include <rla/Png.hpp>

constexpr rl::Png::Color rl::to_png_color(rl::Bitmap::Color color) noexcept
{
    switch (color)
    {
        case rl::Bitmap::Color::G:
            return rl::Png::Color::G;
        case rl::Bitmap::Color::Ga:
            return rl::Png::Color::Ga;
        case rl::Bitmap::Color::Rgb:
            return rl::Png::Color::Rgb;
        case rl::Bitmap::Color::Rgba:
            return rl::Png::Color::Rgba;
    }
    return rl::Png::Color::Rgb;
}

constexpr rl::Bitmap::Color rl::to_bitmap_color(rl::Png::Color color) noexcept
{
    switch (color)
    {
        case rl::Png::Color::G:
            return rl::Bitmap::Color::G;
        case rl::Png::Color::Ga:
            return rl::Bitmap::Color::Ga;
        case rl::Png::Color::Rgb:
            return rl::Bitmap::Color::Rgb;
        case rl::Png::Color::Rgba:
            return rl::Bitmap::Color::Rgba;
        case rl::Png::Color::Palette:
            return rl::Bitmap::Color::Rgb;
    }
    return rl::Bitmap::Color::Rgb;
}