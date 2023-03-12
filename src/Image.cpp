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
#include <rla/Png.hpp>
#include <rla/color_conversion.hpp>
#include <cstring>

void rl::Image::shrink_data()
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

void rl::Image::reserve_data(std::size_t capacity)
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

void rl::Image::free_data() noexcept
{
    delete[] this->data;
    this->capacity = 0;
    this->data = nullptr;
}

rl::Image::Image(const rl::Png& png, std::optional<rl::Bitmap::Depth> depth_o, std::optional<rl::Bitmap::Color> color_o)
{
    this->Load(png, depth_o, color_o);
}

rl::Image::Image(std::string_view path, std::optional<rl::Bitmap::Depth> depth_o, std::optional<rl::Bitmap::Color> color_o)
{
    this->Load(path, depth_o, color_o);
}

rl::Image::Image(std::size_t capacity)
{
    this->reserve_data(capacity);
}

rl::Image::Image(std::size_t width, std::size_t height, std::size_t page_count, rl::Bitmap::Depth depth, rl::Bitmap::Color color)
{
    this->Create(width, height, page_count, depth, color);
}

rl::Image::~Image() noexcept
{
    this->free_data();
}

void rl::Image::Clear() noexcept
{
    this->width = 0;
    this->height = 0;
    this->page_count = 0;
    this->depth = rl::Bitmap::Depth::Default;
    this->color = rl::Bitmap::Color::Default;
}

void rl::Image::ShrinkToFit()
{
    this->shrink_data();
}

void rl::Image::Reserve(std::size_t capacity)
{
    this->reserve_data(capacity);
}

void rl::Image::Create(std::size_t width, std::size_t height, std::size_t page_count, rl::Bitmap::Depth depth, rl::Bitmap::Color color)
{
    this->Clear();
    const auto size = 
        rl::Bitmap::GetSize(
            width,
            height,
            page_count,
            depth,
            color
        );
    this->reserve_data(size);
    this->width = width;
    this->height = height;
    this->page_count = page_count;
    this->color = color;
    this->depth = depth;
}

void rl::Image::Load(const rl::Png& png, std::optional<rl::Bitmap::Depth> depth_o, std::optional<rl::Bitmap::Color> color_o)
{
    this->Create(
        png.GetWidth(),
        png.GetHeight(),
        1,
        depth_o.value_or(
            rl::Bitmap::GetDepth(png.GetBitDepth())
        ),
        color_o.value_or(
            rl::to_bitmap_color(png.GetColor())
        )
    );
    this->Blit(png, 0, 0, 0);
}

void rl::Image::Load(std::string_view path, std::optional<rl::Bitmap::Depth> depth_o, std::optional<rl::Bitmap::Color> color_o)
{
    const auto png = rl::Png(path);
    this->Load(png, depth_o, color_o);
}