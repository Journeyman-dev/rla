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

#include <rla/Image.hpp>

unsigned char* rl::Image::GetMutableData() noexcept
{
    return this->data.data();
}

rl::Bitmap::Color rl::Image::GetColor() const noexcept
{
    return this->color;
}

std::size_t rl::Image::GetWidth() const noexcept
{
    return this->width;
}

std::size_t rl::Image::GetHeight() const noexcept
{
    return this->height;
}

std::size_t rl::Image::GetPages() const noexcept
{
    return this->pages;
}

std::size_t rl::Image::GetChannelSize() const noexcept
{
    return this->channel_size;
}

const unsigned char* rl::Image::GetData() const noexcept
{
    return this->data.data();
}
