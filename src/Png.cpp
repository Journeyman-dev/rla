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
#include <rla/PngException.hpp>
#include "png_wrapper.h"
#include "pngw_ext.hpp"

rl::Png::Png(std::string_view path)
{
    this->Load(path);
}

void rl::Png::Load(std::string_view path)
{
    std::size_t width, height, bit_depth;
    pngwcolor_t color;
    pngwresult_t result = pngwFileInfo(path.data(), &width, &height, &bit_depth, &color);
    if (result != PNGW_RESULT_OK)
    {
        throw rl::PngException(rl::pngw_result_to_png_exception_error(result));
    }
    this->path = path;
    this->width = width;
    this->height = height;
    this->color = rl::pngw_color_to_png_color(color);
    this->bit_depth = bit_depth;
}

bool rl::Png::GetEmpty() const noexcept
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
