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

#include <rla/ImageRow.hpp>
#include <rla/Bitmap.hpp>
#include <vector>

std::size_t rl::ImageRow::GetWidth() const noexcept
{
    return this->width;
}

rl::BitmapDepth rl::ImageRow::GetDepth() const noexcept
{
    return this->depth;
}

rl::BitmapColor rl::ImageRow::GetColor() const noexcept
{
    return this->color;
}

const rl::bitmap_byte_t* rl::ImageRow::GetData() const noexcept
{
    return this->data.data();
}

rl::bitmap_byte_t* rl::ImageRow::GetMutableData() noexcept
{
    return this->data.data();
}

void rl::ImageRow::Clear() noexcept
{
    this->data.clear();
    this->width = 0;
    this->depth = rl::BitmapDepth::Default;
    this->color = rl::BitmapColor::Default;
}

void rl::ImageRow::ShrinkToFit()
{
    this->data.shrink_to_fit();
}

void rl::ImageRow::Reserve(std::size_t capacity)
{
    this->data.reserve(capacity);
}

std::size_t rl::ImageRow::GetCapacity() const noexcept
{
    return this->data.capacity();
}

void rl::ImageRow::Create(std::size_t width, rl::BitmapDepth depth, rl::BitmapColor color)
{
    this->Clear();
    const auto size = 
        rl::Bitmap::GetSize(
            width,
            0,
            0,
            depth,
            color
        );
    this->data.resize(size, 0);
    this->width = width;
    this->color = color;
    this->depth = depth;
}
