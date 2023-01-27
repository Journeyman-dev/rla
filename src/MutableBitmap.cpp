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

#include <rla/MutableBitmap.hpp>
#include <rla/Png.hpp>
#include "png_wrapper.h"
#include "pngw_ext.hpp"
#include <rlm/cellular/box2.hpp>
#include <rla/PngException.hpp>

unsigned char* rl::MutableBitmap::GetMutableData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetMutableData() +
        byte_index_o.value();
}

void rl::MutableBitmap::BlitPng(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page)
{
    rl::box2<int> this_box(
        0,
        0,
        static_cast<int>(this->GetWidth()),
        static_cast<int>(this->GetHeight())
    );
    rl::box2<int> other_box(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(png.GetWidth()),
        static_cast<int>(png.GetHeight())
    );
    if (!rl::contains(this_box, other_box) || page >= this->GetPages())
    {
        throw rl::PngException(rl::PngException::Error::BlitOutOfImage);
    }
    pngwReadFile(
        png.GetPath().data(),
        this->GetMutableData(x, y, page, 0),
        this->GetRowSize(),
        png.GetWidth(),
        png.GetHeight(),
        this->GetBitDepth(),
        rl::bitmap_color_to_pngw_color(this->GetColor())
    );
}

void rl::MutableBitmap::BlitPng(std::string_view path, std::size_t x, std::size_t y, std::size_t page)
{
    rl::Png png(path);
    this->BlitPng(png, x, y, page);
}
