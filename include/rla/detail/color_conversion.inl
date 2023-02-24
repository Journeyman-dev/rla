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

#include <rla/BitmapColor.hpp>
#include <rla/PngColor.hpp>

constexpr rl::PngColor rl::to_png_color(rl::BitmapColor color) noexcept
{
    switch (color)
    {
        case rl::BitmapColor::G:
            return rl::PngColor::G;
        case rl::BitmapColor::Ga:
            return rl::PngColor::Ga;
        case rl::BitmapColor::Rgb:
            return rl::PngColor::Rgb;
        case rl::BitmapColor::Rgba:
            return rl::PngColor::Rgba;
    }
    return rl::PngColor::Rgb;
}

constexpr rl::BitmapColor rl::to_bitmap_color(rl::PngColor color) noexcept
{
    switch (color)
    {
        case rl::PngColor::G:
            return rl::BitmapColor::G;
        case rl::PngColor::Ga:
            return rl::BitmapColor::Ga;
        case rl::PngColor::Rgb:
            return rl::BitmapColor::Rgb;
        case rl::PngColor::Rgba:
            return rl::BitmapColor::Rgba;
        case rl::PngColor::Palette:
            return rl::BitmapColor::Rgb;
    }
    return rl::BitmapColor::Rgb;
}