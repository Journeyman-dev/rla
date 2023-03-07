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

#include <catch2/catch_all.hpp>
#include <rla/color_conversion.hpp>

// clang-format off

TEST_CASE("A rl::Png::Color is converted into a rl::Bitmap::Color")
{
    CHECK(rl::to_bitmap_color(rl::Png::Color::G)       == rl::Bitmap::Color::G);
    CHECK(rl::to_bitmap_color(rl::Png::Color::Ga)      == rl::Bitmap::Color::Ga);
    CHECK(rl::to_bitmap_color(rl::Png::Color::Rgb)     == rl::Bitmap::Color::Rgb);
    CHECK(rl::to_bitmap_color(rl::Png::Color::Rgba)    == rl::Bitmap::Color::Rgba);
    CHECK(rl::to_bitmap_color(rl::Png::Color::Palette) == rl::Bitmap::Color::Rgb);
}

TEST_CASE("A rl::Bitmap::Color is converted into a rl::Png::Color")
{
    CHECK(rl::to_png_color(rl::Bitmap::Color::G)    == rl::Png::Color::G);
    CHECK(rl::to_png_color(rl::Bitmap::Color::Ga)   == rl::Png::Color::Ga);
    CHECK(rl::to_png_color(rl::Bitmap::Color::Rgb)  == rl::Png::Color::Rgb);
    CHECK(rl::to_png_color(rl::Bitmap::Color::Rgba) == rl::Png::Color::Rgba);
}