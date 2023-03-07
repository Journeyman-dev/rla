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

void rl::Image::Row::shrink_data()
{
    if (this->GetSize() == 0 && this->data != nullptr)
    {
        delete[] this->data;
        this->data = nullptr;
    }
    else if (this->capacity < this->GetSize())
    {
        rl::Bitmap::byte_t* new_data = new rl::Bitmap::byte_t[this->GetSize()];
        std::memcpy(new_data, this->data, this->GetSize());
        delete[] this->data;
        this->data = new_data;
        this->capacity = this->GetSize();
    }
}

void rl::Image::Row::reserve_data(std::size_t capacity)
{
    if (capacity > this->capacity)
    {
        rl::Bitmap::byte_t* new_data = new rl::Bitmap::byte_t[capacity];
        std::memcpy(new_data, this->data, this->GetSize());
        delete[] this->data;
        this->data = new_data;
        this->capacity = capacity;
    }
}

void rl::Image::Row::free_data() noexcept
{
    delete[] this->data;
    this->capacity = 0;
    this->data = nullptr;
}

rl::Image::Row::Row(std::size_t capacity)
{
    this->reserve_data(capacity);
}

rl::Image::Row::Row(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color)
{
    this->Create(width, depth, color);
}

rl::Image::Row::~Row() noexcept
{
    this->free_data();
}

void rl::Image::Row::Clear() noexcept
{
    this->width = 0;
    this->depth = rl::Bitmap::Depth::Default;
    this->color = rl::Bitmap::Color::Default;
}

void rl::Image::Row::ShrinkToFit()
{
    this->shrink_data();
}

void rl::Image::Row::Reserve(std::size_t capacity)
{
    this->reserve_data(capacity);
}   

std::size_t rl::Image::Row::GetCapacity() const noexcept
{
    return this->capacity;
}

void rl::Image::Row::Create(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color)
{
    this->Clear();
    const auto size = 
        rl::Bitmap::GetRowSize(
            width,
            depth,
            color
        );
    this->reserve_data(size);
    this->width = width;
    this->color = color;
    this->depth = depth;
}